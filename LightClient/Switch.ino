/*
Name:		DomoIno.ino
Created:	9/30/2017 4:08:32 PM
Author:	Alberto Geniola
*/
#include "SimpleSwitch.h"
#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>
#include "AuthTable.h"
#include "AbstractDevice.h"
#include "SimpleSwitch.h"

//*********************************************************************************************
// *********** IMPORTANT SETTINGS - YOU MUST CHANGE/ONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define NETWORKID   200		// The same on all nodes that talk to each other
#define ME			2		// The unique identifier of this node
#define REMOTE      1		// The recipient of packets

#define FREQUENCY     RF69_433MHZ			//Match frequency to the hardware version of the radio on your device
#define ENCRYPTKEY    "sampleEncryptKey"	//exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HCW   true					// set to 'true' if you are using an RFM69HCW module

// ESP8266
#define SERIAL_BAUD   115200
#define RFM69_CS      15  // GPIO15/HCS/D8
#define RFM69_IRQ     4   // GPIO04/D2
#define RFM69_IRQN    digitalPinToInterrupt(RFM69_IRQ)
#define RFM69_RST     2   // GPIO02/D4

// Radio
RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);

// Message buffer
Msg msg;

// Controller device
SimpleSwitch ctrl(ME, "Simple switch example",&radio);

// the setup function runs once when you press reset or power the board
void setup() {

	// Initialize Radio
	Serial.begin(SERIAL_BAUD);
	Serial.println("Setting up...");
	
	// Hard Reset the RFM module
	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, HIGH);
	delay(100);
	digitalWrite(RFM69_RST, LOW);
	delay(100);

	// Initialize radio
	if (!radio.initialize(FREQUENCY, ME, NETWORKID)) {
		Serial.println("radio.initialize failed!");
	}
	if (IS_RFM69HCW) {
		radio.setHighPower();    // Only for RFM69HCW & HW!
	}
	radio.setPowerLevel(31); // power output ranges from 0 (5dBm) to 31 (20dBm)

	radio.encrypt(ENCRYPTKEY);

	Serial.print("\nTransmitting at ");
	Serial.print(FREQUENCY == RF69_433MHZ ? 433 : FREQUENCY == RF69_868MHZ ? 868 : 915);
	Serial.println(" MHz");
	Serial.print("Network "); Serial.print(NETWORKID);
	Serial.print(" Node "); Serial.println(ME); Serial.println();
}

// the loop function runs over and over again until power down or reset
void loop() {
	msg.erase();
	
	// We operate as server. Our duty is to wait for message and handle them.
	switch (ctrl.recv(msg)) {
	
	// When we get an invalid we will usually reply "hey, you did use an invalid nonce".
	// Since this behaviour may be dependent on the specific device we are going to implement,
	// we let the class to override this method. The default behavior is to "answer" a NACK
	// to notify the client/server it is out of sync. 
	case RecvResult::MSG_INVALID_NONCE:
		Serial.println("\n----------------------------------");
		ctrl.handleInvalidNonce(msg);
		Serial.println("\n----------------------------------\n");
		break;
	case RecvResult::MSG_TIMEOUT:
		Serial.print(".");
		break;
	case RecvResult::MSG_OK:
		Serial.println("\n----------------------------------");
		ctrl.handleMsg(msg);
		Serial.println("\n----------------------------------\n");
		break;
	}
	delay(100);

}
