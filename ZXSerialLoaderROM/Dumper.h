#pragma once

#include "arduino.h"

#define DUMP_SEGMENT_ADDRESS 164
#define DUMP_EXIT_ADDRESS 163

const byte PROGMEM DUMPProgram[] =
{

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xED,0x73,0x17,0x00,0x31,0x65,0x00,0x3E,0x06,0xD3,
    0xFE,0x2A,0x1C,0x00,0x11,0xA4,0x00,0xED,0x4B,0x1E,0x00,0xED,0xB0,0x3E,0x01,0x32,
    0x1B,0x00,0x3E,0x01,0xD3,0xFE,0x3A,0x1B,0x00,0xEE,0xAA,0xCA,0x9E,0x00,0x3A,0x1B,
    0x00,0xEE,0x55,0xC2,0x86,0x00,0x3E,0x00,0x32,0x1B,0x00,0xC3,0x6D,0x00,0xED,0x7B,
    0x17,0x00,0xED,0x45,
}