#pragma once

#include "Arduino.h"

#define STATUS_ADDRESS			0
#define START_ADDRESS_ADDRESS	1
#define SEGMENT_DEST_ADDRESS	3
#define SEGMENT_SIZE_ADDRESS	5

#define EXIT_ADDRESS			0x98
#define SEGMENT_ADDRESS			0x99

const byte HEXProgram[] PROGMEM = { 0x00 };