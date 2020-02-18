/*
 Name:		ZXBasicPeripheral.ino
 Created:	05/02/2020 10:17:40
 Author:	geniw
*/

#include <arduino.h>
#include "ZXShield.h"

// the setup function runs once when you press reset or power the board
void setup() {

	ZXShield::Initialize();

	ZXShield::Peripheral.CreateRegister(0, BYTE_R, true, true);
	ZXShield::Peripheral.CreateRegister(1, INT_R, true, true);
	ZXShield::Peripheral.CreateRegister(2, STREAM_R, true, true);
	ZXShield::EnablePeripheral();
	ZXShield::ResetCPU();
}

// the loop function runs over and over again until power down or reset
void loop() {
	
}
