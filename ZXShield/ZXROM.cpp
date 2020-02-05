#include "ZXROM.h"

void ZXROM::SetROM(const PROGMEM byte* ROMData)
{
	data = ROMData;
}

void ZXROM::SetROMHandler(ROM_HANDLER Handler)
{
	handler = Handler;
}

void ZXROM::DataReady(unsigned int Address, byte Operation)
{
	if (data != NULL && !Operation)
		ZXShield::OutputROMByte(pgm_read_byte(&data[Address]));
	else if (handler != NULL)
		handler(Address, Operation);
	else
		ZXShield::ReleaseTrap();
}
