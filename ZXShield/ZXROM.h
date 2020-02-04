#pragma once
#include <Arduino.h>
#include "ZXShield.h"

typedef void (*ROM_HANDLER)(unsigned int Address, byte Operation);

class ZXROM
{
public:
	void SetROM(const PROGMEM byte* ROMData);
	void SetROMHandler(ROM_HANDLER Handler);
	void DataReady(unsigned int Address, byte Operation);
private:
	ROM_HANDLER handler;
	const PROGMEM byte* data;
};

