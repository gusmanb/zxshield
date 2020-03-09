#pragma once
#include <Arduino.h>
#include <SPI.h>

#define MIDI_CMD_NOTEOFF 0x80
#define MIDI_CMD_NOTEON 0x90
#define MIDI_CMD_AFT 0xA0
#define MIDI_CMD_CC 0xB0
#define MIDI_CMD_PATCH 0xC0
#define MIDI_CMD_PRESS 0xD0
#define MIDI_CMD_PITCH 0xE0


#define MIDI_CC_BANK 0x00
#define MIDI_CC_MOD 0x01
#define MIDI_CC_BREATH 0x02
#define MIDI_CC_VOL 0x07
#define MIDI_CC_PAN 0x0A
#define MIDI_CC_EXPR 0x0B
#define MIDI_CC_SUST 0x40
#define MIDI_CC_PORTA 0x41
#define MIDI_CC_RESO 0x47

#define SCI_MODE 0x0
#define SCI_VOL 0xB

#define CHANNEL_CMD(cmd,ch,p1) ({\
\
while (!digitalRead(dreq)); \
digitalWrite(xdcs, 0); \
SPI.beginTransaction(vsSettings); \
SPI.transfer16(cmd | ch); \
SPI.transfer16(p1); \
SPI.endTransaction(); \
digitalWrite(xdcs, 1); \
\
})
#define CHANNEL_CMD_2(cmd,ch,p1,p2) ({\
\
while (!digitalRead(dreq)); \
digitalWrite(xdcs, 0); \
SPI.beginTransaction(vsSettings); \
SPI.transfer16(cmd | ch); \
SPI.transfer16(p1); \
SPI.transfer16(p2); \
SPI.endTransaction(); \
digitalWrite(xdcs, 1); \
\
})



class VS1053MIDI
{
public:
	VS1053MIDI(byte CS, byte XCS, byte XRESET, byte XDCS, byte DREQ, byte RTL);

	void NoteOn(byte Channel, byte Note, byte Velocity);
	void NoteOff(byte Channel, byte Note, byte Velocity);
	void Aftertouch(byte Channel, byte Note, byte Touch);
	void Patch(byte Channel, byte Patch);
	void Pressure(byte Channel, byte Pressure);
	void Pitch(byte Channel, word Pitch);

	void ModulationController(byte Channel, byte Value);
	void VolumeController(byte Channel, byte Value);
	void BreathController(byte Channel, byte Value);
	void PanController(byte Channel, byte Value);
	void ExpressionController(byte Channel, byte Value);
	void PortamentoController(byte Channel, byte Value);
	void SustainedController(byte Channel, byte Value);
	void ResonanceController(byte Channel, byte Value);
	void OtherController(byte Controller, byte Channel, byte Value);

	void EarSpace(bool On);
	void Volume(byte Left, byte Right);

	void SendMIDICommand(byte Command, byte Param1);
	void SendMIDICommand(byte Command, byte Param1, byte Param2);

	void WriteSCIRegister(byte Register, word Value);
	word ReadSCIRegister(byte Register);

	void Reset();
private:
	byte cs;
	byte xcs;
	byte xreset;
	byte xdcs;
	byte dreq;
	byte rtl;
	
};

