/*
 Name:		ZXSwappableROM.ino
 Created:	05/02/2020 13:39:06
 Author:	geniw
*/
#include "arduino.h"
#include "ZXShield.h"
#include "48gusrom.h"

bool newOp = false;
byte selectedRom = 0;

// the setup function runs once when you press reset or power the board
void setup() {
	ZXShield::Initialize();
	ZXShield::ROM.SetROM(ROMData);
	ZXShield::Peripheral.CreateRegister(0, BYTE_R, true, true, NULL, NULL, NULL, NULL, CheckRom);
	ZXShield::EnablePeripheral();
	ZXShield::ResetCPU();
}

void CheckRom(PREGISTER Target)
{
	selectedRom = ZXPeripheral::GetByteValue(Target);
	newOp = true;
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	if (newOp)
	{
		newOp = false;

		if (selectedRom == 0)
		{
			ZXShield::DisableROM();
		}
		else
		{
			ZXShield::EnableROM();
		}

		ZXShield::ResetCPU();
		
	}

}
