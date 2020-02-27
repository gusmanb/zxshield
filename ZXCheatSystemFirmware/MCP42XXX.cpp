// 
// 
// 

#include "MCP42XXX.h"

static SPISettings digipotSettings(2000000, MSBFIRST, SPI_MODE0);

MCP42XXX::MCP42XXX(int CS) 
{
	_CS = CS;

	pinMode(_CS, OUTPUT);
	SPI.begin();
}

void MCP42XXX::SetPot(byte pot, byte value) 
{
	SPI.beginTransaction(digipotSettings);
	digitalWrite(_CS, LOW);
	SPI.transfer((pot & B11) | B00010000);
	SPI.transfer(value);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}
