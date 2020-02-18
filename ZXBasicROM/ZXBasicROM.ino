/*
 Name:		ZXBasicROM.ino
 Created:	04/02/2020 10:47:09
 Author:	geniw
*/

#include "ZXShield.h"
#include "48gusrom.h"

// the setup function runs once when you press reset or power the board
void setup() {

	ZXShield::Initialize();
	ZXShield::ROM.SetROM(ROMData);
	ZXShield::EnableROM();
	ZXShield::ResetCPU();
	
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
