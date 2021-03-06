#pragma once

#include "arduino.h"

#define TAP_SEGMENT_ADDRESS 176
#define TAP_EXIT_ADDRESS 175

#define TAP_STATUS_ADDRESS			0
#define TAP_START_ADDRESS_ADDRESS	1
#define TAP_SEGMENT_DEST_ADDRESS	3
#define TAP_SEGMENT_SIZE_ADDRESS	5

const byte PROGMEM TAPProgram[] =
{

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0x21,0xB0,0x00,0xED,0x5B,0x03,0x00,0xED,0x4B,
    0x05,0x00,0xED,0xB0,0x3E,0x01,0x32,0x00,0x00,0x3A,0x00,0x00,0xEE,0xAA,0xCA,0x91,
    0x00,0x3A,0x00,0x00,0xEE,0x55,0xC2,0x79,0x00,0x3E,0x00,0x32,0x00,0x00,0xC3,0x67,
    0x00,0x31,0x19,0x75,0x01,0x54,0x00,0xC5,0xED,0x4B,0x01,0x00,0xC5,0x11,0x1F,0x5D,
    0xDD,0x21,0x00,0x00,0xFD,0x21,0x3A,0x5C,0x3E,0x00,0xED,0x4F,0xF1,0xFB,0xED,0x45,
};