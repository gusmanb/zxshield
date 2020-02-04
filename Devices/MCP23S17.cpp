#include "MCP23S17.h"

static SPISettings expanderSettings(2000000, MSBFIRST, SPI_MODE0);


MCP23S17::MCP23S17(int Address, int CS)
{
	addr = Address;
	cs = CS;
	::pinMode(cs, OUTPUT);
	::digitalWrite(cs, HIGH);
	SPI.begin();
	WriteRegister(IOCON, ADDR_ENABLE);
	
}

void MCP23S17::WriteRegister(EXPANDER_REGISTERS Register, byte Value)
{
	SPI.beginTransaction(expanderSettings);
	::digitalWrite(cs, LOW);                            // Take slave-select low
	SPI.transfer(EP_WRITE | (addr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(Register);                                   // Send the register we want to write
	SPI.transfer(Value);                                 // Send the byte
	::digitalWrite(cs, HIGH);                           // Take slave-select high
	SPI.endTransaction();
}

byte MCP23S17::ReadRegister(EXPANDER_REGISTERS Register)
{
	SPI.beginTransaction(expanderSettings);
	::digitalWrite(cs, LOW);                            // Take slave-select low
	SPI.transfer(EP_READ | (addr << 1));             // Send the MCP23S17 opcode, chip address, and write bit
	SPI.transfer(Register);                                   // Send the register we want to write
	byte value = SPI.transfer(0xFF);                                 // Send the byte
	::digitalWrite(cs, HIGH);
	SPI.endTransaction();

	return value;
	
}

void MCP23S17::SetDirections(EXPANDER_PORTS Port, byte Directions)
{
	if (Port == E_PORTA)
		WriteRegister(IODIRA, Directions);
	else
		WriteRegister(IODIRB, Directions);
}

void MCP23S17::SetPullups(EXPANDER_PORTS Port, byte Pullups)
{
	if (Port == E_PORTA)
		WriteRegister(GPPUA, Pullups);
	else
		WriteRegister(GPPUB, Pullups);
}

void MCP23S17::SetPolarities(EXPANDER_PORTS Port, byte Polarities)
{
	if (Port == E_PORTA)
		WriteRegister(IPOLA, Polarities);
	else
		WriteRegister(IPOLB, Polarities);
}

void MCP23S17::WritePort(EXPANDER_PORTS Port, byte Value)
{
	if (Port == E_PORTA)
		WriteRegister(GPIOA, Value);
	else
		WriteRegister(GPIOB, Value);
}

byte MCP23S17::ReadPort(EXPANDER_PORTS Port)
{
	if (Port == E_PORTA)
		return ReadRegister(GPIOA);
	else
		return ReadRegister(GPIOB);
}


MCP23S17::~MCP23S17()
{
}
