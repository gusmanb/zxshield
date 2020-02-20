/*
 Name:		ZXPokeadorFirmware.ino
 Created:	19/02/2020 0:38:32
 Author:	geniw
*/

#include "arduino.h"
#include "ZXShield.h"
#include "ZXPokeadorROM.h"

volatile byte virtualRAM[4096];
volatile byte vramDisabled = 1;

#define NMI_BUTTON 13

// the setup function runs once when you press reset or power the board
void setup() {

    memset((void*)virtualRAM, 0, 4096);
    memcpy_P((void*)virtualRAM, POKEProgram, POKE_ROM_LENGTH);

	pinMode(NMI_BUTTON, INPUT_PULLUP);

    ZXShield::Initialize();
    ZXShield::ROM.SetROMHandler(PokeadorHandler);
    ZXShield::ResetCPU();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	if (vramDisabled)
	{
		if (!digitalRead(NMI_BUTTON))
		{
			vramDisabled = 0;
			ZXShield::EnableROMWithNMI();
		}
	}
  
}

void PokeadorHandler(word Address, byte Operation)
{
	//Write?
	if (Operation)
		virtualRAM[Address] = ZXShield::InputByte();//Store data
	else
	{
		if (Address == POKE_EXIT_ADDRESS) //Did the speccy read the last instruction?
		{
			//disable vRAM
			vramDisabled = 1;
			ZXShield::DisableROM();
		}

		ZXShield::OutputROMByte(virtualRAM[Address]); //send data

	}
}