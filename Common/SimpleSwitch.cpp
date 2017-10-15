// 
// 
// 

#include "SimpleSwitch.h"

// Constructor
SimpleSwitch::SimpleSwitch(uint8_t nodeId, char* nodeDescr, RFM69* radio) : AbstractDevice(DeviceClassId::SIMPLE_SWITCH, nodeId, nodeDescr, radio) {};

void SimpleSwitch::handleSpecificMsg(Msg& msg) {
	// TODO...
}