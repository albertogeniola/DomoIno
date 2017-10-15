// AbstractDevice.h

#ifndef _ABSTRACTDEVICE_h
#define _ABSTRACTDEVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Msg.h"
#include "ProtocolConstants.h"
#include "AuthTable.h"

enum RecvResult
{
	MSG_OK,
	MSG_TIMEOUT,
	MSG_INVALID_NONCE
};

class AbstractDevice {
private:	
	// This is the radio used by the device.
	// The caller should pass a reference to an
	// already configured radio.
	RFM69* radio;
	AuthTable authTable;

protected:
	// ID used to identify the type of the device
	DeviceClassId deviceClassId;
	uint8_t nodeId;
	char nodeDescription[MAX_NODE_DESCR_LEN];

	// This is the method that all subclasses should implement in order to process specific messages.
	virtual void handleSpecificMsg(Msg& msg) = 0;

public:
	// Base constructor
	AbstractDevice(DeviceClassId deviceClassId, uint8_t nodeId, char* nodeDescr, RFM69* radio);
	
	// Function used to receive messages from the radio
	RecvResult recv(Msg& msg);

	// Function used to send messages throught the radio
	bool send(Msg& msg, uint16_t overrideRemoteNonce);

	// Main message handler. Handles generic messaging and invokes handleInvalidNonce. When the kind of message
	// is not handled by this method, handleSpecificMsg is invoked.
	void handleMsg(Msg& msg);

	virtual void handleInvalidNonce(Msg& msg);
	
};

#endif