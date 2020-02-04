// MCP23S17.h

#ifndef _MCP23S17_h
#define _MCP23S17_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#include <SPI.h>

#else
#include "WProgram.h"
#endif

#define    ADDR_ENABLE   (0b00001000)

enum EXPANDER_REGISTERS
{
    IODIRA =   (0x00),      // MCP23x17 I/O Direction Register
    IODIRB = (0x01),      // 1 = Input (default), 0 = Output

    IPOLA = (0x02),     // MCP23x17 Input Polarity Register
    IPOLB = (0x03),      // 0 = Normal (default)(low reads as 0), 1 = Inverted (low reads as 1)

    GPINTENA = (0x04),      // MCP23x17 Interrupt on Change Pin Assignements
    GPINTENB = (0x05),      // 0 = No Interrupt on Change (default), 1 = Interrupt on Change

    DEFVALA = (0x06),      // MCP23x17 Default Compare Register for Interrupt on Change
    DEFVALB = (0x07),      // Opposite of what is here will trigger an interrupt (default = 0)

    INTCONA = (0x08),      // MCP23x17 Interrupt on Change Control Register
    INTCONB = (0x09),      // 1 = pin is compared to DEFVAL, 0 = pin is compared to previous state (default)

    IOCON = (0x0A),      // MCP23x17 Configuration Register
    IOCON2 = (0x0B),      //     Also Configuration Register

    GPPUA = (0x0C),      // MCP23x17 Weak Pull-Up Resistor Register
    GPPUB = (0x0D),      // INPUT ONLY: 0 = No Internal 100k Pull-Up (default) 1 = Internal 100k Pull-Up 

    INTFA = (0x0E),      // MCP23x17 Interrupt Flag Register
    INTFB = (0x0F),      // READ ONLY: 1 = This Pin Triggered the Interrupt

    INTCAPA = (0x10),      // MCP23x17 Interrupt Captured Value for Port Register
    INTCAPB = (0x11),      // READ ONLY: State of the Pin at the Time the Interrupt Occurred

    GPIOA = (0x12),      // MCP23x17 GPIO Port Register
    GPIOB = (0x13),      // Value on the Port - Writing Sets Bits in the Output Latch

    OLATA = (0x14),      // MCP23x17 Output Latch Register
    OLATB = (0x15)      // 1 = Latch High, 0 = Latch Low (default) Reading Returns Latch State, Not Port Value!
};

enum EXPANDER_OPS
{
	EP_WRITE = 0b01000000,
	EP_READ = 0b01000001
};

enum EXPANDER_PORTS
{
	E_PORTA,
	E_PORTB
};

class MCP23S17
{
public:
	MCP23S17(int Address, int CS);
	~MCP23S17();
	void WriteRegister(EXPANDER_REGISTERS Register, byte Value);
	byte ReadRegister(EXPANDER_REGISTERS Register);
	void SetDirections(EXPANDER_PORTS Port, byte Directions);
	void SetPullups(EXPANDER_PORTS Port, byte Pullups);
	void SetPolarities(EXPANDER_PORTS Port, byte Polarities);
	void WritePort(EXPANDER_PORTS Port, byte Value);
	byte ReadPort(EXPANDER_PORTS Port);

private:
	int addr;
	int cs;
};

#endif
