#pragma once

#include "arduino.h"

#define Z80_SEGMENT_ADDRESS 253
#define Z80_EXIT_ADDRESS 252

#define Z80_HEADER_ADDRESS			0
#define Z80_HEADER_SIZE				29
#define Z80_STATUS_ADDRESS			0 + Z80_HEADER_SIZE
#define Z80_START_ADDRESS_ADDRESS	1 + Z80_HEADER_SIZE
#define Z80_SEGMENT_DEST_ADDRESS	3 + Z80_HEADER_SIZE
#define Z80_SEGMENT_SIZE_ADDRESS	5 + Z80_HEADER_SIZE

const byte PROGMEM Z80Program[] =
{

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0xE1,0x21,0xFD,0x00,0xED,0x5B,0x20,0x00,0xED,
	0x4B,0x22,0x00,0xED,0xB0,0x3E,0x01,0x32,0x1D,0x00,0x3A,0x1D,0x00,0xEE,0xAA,0xCA,
	0x92,0x00,0x3A,0x1D,0x00,0xEE,0x55,0xC2,0x7A,0x00,0x3E,0x00,0x32,0x1D,0x00,0xC3,
	0x68,0x00,0xED,0x7B,0x17,0x00,0x3A,0x1A,0x00,0xD3,0xFE,0x2A,0x01,0x00,0xED,0x5B,
	0x03,0x00,0xED,0x4B,0x05,0x00,0xD9,0xED,0x4B,0x1B,0x00,0xC5,0xED,0x4B,0x15,0x00,
	0xC5,0xED,0x4B,0x07,0x00,0xC5,0x2A,0x09,0x00,0xED,0x5B,0x0B,0x00,0xED,0x4B,0x0D,
	0x00,0xFD,0x2A,0x0F,0x00,0xDD,0x2A,0x11,0x00,0x3A,0x19,0x00,0xEE,0x02,0xC2,0xD6,
	0x00,0xED,0x5E,0xC3,0xE5,0x00,0x3A,0x19,0x00,0xEE,0x01,0xC2,0xE3,0x00,0xED,0x56,
	0xC3,0xE5,0x00,0xED,0x46,0x3A,0x13,0x00,0xF6,0x00,0xCA,0xEE,0x00,0xFB,0x3A,0x00,
	0x00,0xED,0x47,0xF1,0x08,0x3A,0x14,0x00,0xED,0x4F,0xF1,0xED,0x45,
};