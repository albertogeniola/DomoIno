// Controller.h

#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RFM69.h>
#include "Msg.h"
#include "AbstractDevice.h"

	class Controller : public AbstractDevice {
public:
	void handleSpecificMsg(Msg& msg);
	Controller(char* nodeDescr, RFM69* radio);
};

#endif

