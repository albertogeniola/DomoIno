// AuthTable.h

#ifndef _AUTHTABLE_h
#define _AUTHTABLE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ProtocolConstants.h"

#define RANDOM_REG32  ESP8266_DREG(0x20E44)

typedef struct AUTH_ROW {	
	// Which nonce we expect it to provide
	uint16_t nonceExpectedFrom;

	// Which nonce we are expected to provide
	uint16_t nonceExpectedFor;
} AuthRow;

class AuthTable {
private:
	AUTH_ROW rows[MAX_NODES];
public:
	// Constructor
	AuthTable();

	uint16_t getExpectedTokenFor(uint8_t remoteNode);
	void setExpectedTokenFor(uint8_t remoteNode, uint16_t nonce);

	uint16_t getExpectedTokenFrom(uint8_t remoteNode);
	uint16_t updateExpectedTokenFrom(uint8_t remoteNode);
};


#endif

