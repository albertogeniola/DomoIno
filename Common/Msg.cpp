// 
// 
// 

#include "Msg.h"

Msg::Msg() {
	this->recipient = 0;
	this->sender = 0;
	bzero((uint8_t*)&(this->data), sizeof(RawMsg));
	this->data.authHeader.msgType = MsgType::UNKNOWN;
};

void Msg::erase() {
	this->recipient = 0;
	this->sender = 0;
	bzero(&this->data, sizeof(RawMsg));
};

void Msg::buildNACK() {
	/*
		The NACK message is sent by B to A, when A attempted to communicate with B by specifying an
		invalid nonce. In order to let A know its attempt failed, B sends the NACK message and does not
		serve the request of A.

		This messages is identified just by its header. No payload is sent.
		|AUTH_HEADER|
	*/

	bzero(&(this->data.payload), sizeof(RawMsg::payload));
	this->data.authHeader.msgType = MsgType::NACK;
	
	// This message brings no payload!
	this->payloadSize = 0;
};

void Msg::buildHelo() {
	/*
		The helo message is used by devices to discover other devices in the network and to retrieve their
		basic information. 

		This messages is identified just by its header. No payload is sent.
		|AUTH_HEADER|
	*/
	bzero(&(this->data.payload), sizeof(RawMsg::payload));
	this->data.authHeader.msgType = MsgType::HELO;

	// This message brings no payload!
	this->payloadSize = 0;
};

void Msg::buildHeloYou(uint8_t classId, uint8_t nodeId, char* description ) {
	/*
		This message is sent in response to HELO requests. Such message type contains general information about
		the current device, such as: 
		-> CLASS (Device Type)			: uint8_t :  1 byte
		-> CONFIGURED_ADDRESS (Node ID) : uint8_t :  1 byte
		-> DESCRIPTION					: char[48]: 48 bytes

		The structure of the message is the following.
		|AUTH_HEADER|CLASS|CONFIGURED_ADDRESS|DESCRIPTION|
	*/
	bzero(&(this->data.payload), sizeof(RawMsg::payload));
	this->data.authHeader.msgType = MsgType::HELO_YOU;

	// Fill payload
	this->data.payload[0] = classId;
	this->data.payload[1] = nodeId;
	strncpy((char*)this->data.payload + 2, description, 48);
	this->data.payload[50] = '\0';

	// Lenght is given by classId (1) + nodeId (1) + LEN of device name
	this->payloadSize = 2 + strlen((char*)this->data.payload + 2) + 1;
};