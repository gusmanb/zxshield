// MCP42XXX.h

#ifndef _MCP42XXX_h
#define _MCP42XXX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include <SPI.h>

#else
	#include "WProgram.h"
#endif

class MCP42XXX {
public:
	// These are the pins used to connect to the chip
	MCP42XXX(int CS);
	// Sets the given pot (1 or 2) to a given value (0-255)
	void SetPot(byte pot, byte value);
private:
	int _CS;
};

#endif

