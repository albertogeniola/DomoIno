// 
// 
// 

#include "Controller.h"

Controller::Controller(char* nodeDescr, RFM69* radio) : AbstractDevice(DeviceClassId::CONTROLLER, CONTROLLER_NODE_ID, nodeDescr, radio) {};

void Controller::handleSpecificMsg(Msg& msg) {
	Serial.printf("\n------\nMessage from %u to %u.\n", msg.sender, msg.recipient);
	//Serial.printf("\n------\nContent: %s\n------\n", msg.data);
};