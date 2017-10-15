// 
// 
// 
#include <RFM69.h>
#include "AbstractDevice.h"

AbstractDevice::AbstractDevice(DeviceClassId deviceClassId, uint8_t nodeId, char* nodeDescr, RFM69* radio) {
	this->deviceClassId = deviceClassId;
	this->nodeId = nodeId;
	strncpy(this->nodeDescription, nodeDescr, MAX_NODE_DESCR_LEN);
	this->nodeDescription[MAX_NODE_DESCR_LEN - 1] = '\0';
	this->radio = radio;
};

// This method could be overridden by child classes. 
// By default we will only reply to the sender that its nonce
// is invalid.
void AbstractDevice::handleInvalidNonce(Msg& msg) {
	Serial.printf("[XXX] Invalid nonce: %u from %u. Next nonce for this device: %u\n", msg.data.authHeader.nonce, msg.sender, msg.data.authHeader.nextNonce);
	uint16_t nonce = msg.data.authHeader.nextNonce;
	uint16_t recipient = msg.sender;

	msg.erase();
	msg.recipient = recipient;

	// Write NACK
	msg.buildNACK();

	// In this case, we want to override the nonce.
	send(msg, nonce);
	
}

RecvResult AbstractDevice::recv(Msg& msg) {
	// Clean message data
	msg.erase();
	
	// If a message has been received, parse it right away
	// TODO: should we implement wait-timeout?
	if (this->radio->receiveDone()) {
		// Copy the received message into the Msg Buffer
		memcpy(&(msg.data), (uint8_t*)(radio->DATA), radio->DATALEN);
		msg.sender = radio->SENDERID;
		msg.recipient = radio->TARGETID;
		msg.payloadSize = radio->DATALEN - sizeof(RawMsg::authHeader);

		if (radio->ACKRequested())
			radio->sendACK();

		Serial.printf("RECEIVED \"%s\" from %u\nNonce: %u\nExpected: %u\nDataLen: %u\nData: ", msg.data.payload, msg.sender, msg.data.authHeader.nonce, msg.data.authHeader.nextNonce, sizeof(RawMsg));
		for (int i = 0; i < sizeof(RawMsg); i++) {
			uint8_t * addr = (uint8_t*)&(msg.data);
			Serial.printf("%02X ", addr[i]);
		}
		Serial.println();
	}
	else {
		return RecvResult::MSG_TIMEOUT;
	}
	
	// Message was received. Validate it.
	uint16_t expectedNonce = this->authTable.getExpectedTokenFrom(msg.sender);

	if (expectedNonce == 0 || expectedNonce != msg.data.authHeader.nonce)
		return RecvResult::MSG_INVALID_NONCE;

	// Otherwise, it means the message is valid and we want to save its next_nonce for future usage
	this->authTable.setExpectedTokenFor(msg.sender, msg.data.authHeader.nextNonce);

	// At this point, we are sure the message is valid. 
	return RecvResult::MSG_OK;

};

bool AbstractDevice::send(Msg& msg, uint16_t remoteNonce) {

	// Let's retrieve the nonce for the remote nonce if none was specified
	if (remoteNonce == 0)
		remoteNonce = this->authTable.getExpectedTokenFor(msg.recipient);

	msg.data.authHeader.nonce = remoteNonce;

	// Calculate andupdate the expected nonce we would like to receive when the remote node
	// talks again with us
	msg.data.authHeader.nextNonce = this->authTable.updateExpectedTokenFrom(msg.recipient);

	// The total amount of data to send is calculated as the sum of the header and its paylodlen 
	uint8_t sendSize = sizeof(RawMsg::authHeader) + msg.payloadSize;

	Serial.printf("SENDING \"%s\" to %u\nNonce: %u\nExpected: %u\nDataLen: %u\nData: ", msg.data.payload, msg.recipient, msg.data.authHeader.nonce, msg.data.authHeader.nextNonce, sendSize);
	for (int i = 0; i < sendSize; i++) {
		uint8_t * addr = (uint8_t*) &(msg.data);
		Serial.printf("%02X ", addr[i]);
	}
	Serial.println();
	// We can now send the message right away
	return radio->sendWithRetry(msg.recipient, &(msg.data), sendSize);
};

// This function handles protocol reaction for standard messages 
// that are common to all the devices of the network.
void AbstractDevice::handleMsg(Msg& msg) {
	switch (msg.data.authHeader.msgType) {
	case MsgType::HELO:
		// When we receive a HELO request, let's respond with an HELO_YOU!
		msg.buildHeloYou(deviceClassId, nodeId, nodeDescription);
		send(msg, 0);
		break;
	case MsgType::NACK:
		// TODO: handle me!
		Serial.println("Received NACK!!");
		break;
	default:
		return this->handleSpecificMsg(msg);
	}
}