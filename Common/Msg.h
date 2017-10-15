// Msg.h

#ifndef _MSG_h
#define _MSG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


#include <SPI.h>
#include <RFM69.h>

enum MsgType: uint8_t {
	UNKNOWN,	// Default message, which should never be used anywhere
	NACK,		// Used when receiving an unauthorized msg
	HELO,		// Message used by clients to discover other clients on the network. Can be also broadcasted.
	HELO_YOU	// Response to HELO messages.
};

typedef struct MSG_AUTH {
	// Authorization Nonce. Should match the nonce received in the last message
	uint16_t nonce = 0;
	// Expected nonce for the next transmission
	uint16_t nextNonce = 0;
	// Type of the message
	uint8_t msgType;
} MsgAuth;

typedef struct MSG_STRUCT {
	MsgAuth authHeader;
	uint8_t payload[RF69_MAX_DATA_LEN - sizeof(MsgAuth) - 2];
} RawMsg;

// General Message class
class Msg {
public:
	// Used to specify how long should the payload be.
	// This value is used by send in order to send as less bytes as possible
	// and save time during transmission
	uint8_t payloadSize;

	RawMsg data;
	uint8_t recipient;
	uint8_t sender;

	Msg();

	// Builds a NACK message
	void buildNACK();
	
	// Discovery message. Used to get general info about a device
	void buildHelo();

	// Response to Helo Messages. Provides basic info.
	void buildHeloYou(uint8_t classId, uint8_t nodeId, char* description);

	// Cleares the content of the entire message
	void erase();
};



#endif