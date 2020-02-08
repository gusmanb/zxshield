#pragma once

#include "Arduino.h"

#define SNA_STATUS_ADDRESS			0
#define SNA_START_ADDRESS_ADDRESS	1
#define SNA_SEGMENT_DEST_ADDRESS	3
#define SNA_SEGMENT_SIZE_ADDRESS	5

#define SNA_EXIT_ADDRESS			0x98
#define SNA_SEGMENT_ADDRESS			0x99

const byte SNAProgram[] PROGMEM = { 0x00 };