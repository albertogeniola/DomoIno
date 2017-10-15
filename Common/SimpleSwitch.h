// SimpleSwitch.h

#ifndef _SIMPLESWITCH_h
#define _SIMPLESWITCH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AbstractDevice.h"

class SimpleSwitch : public AbstractDevice {
protected:
	void handleSpecificMsg(Msg& msg);
public:
	SimpleSwitch(uint8_t nodeId, char* nodeDescr, RFM69* radio);
};


#endif

