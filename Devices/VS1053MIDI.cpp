#include "VS1053MIDI.h"

//// Plugin to put VS10XX into realtime MIDI mode
//const unsigned short sVS1053b_Realtime_MIDI_Plugin[28] = { /* Compressed plugin */
//  0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /*    0 */
//  0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /*    8 */
//  0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /*   10 */
//  0x0200, 0x000a, 0x0001, 0x0050,
//};

//void VSLoadUserCode(void) {
//    int i = 0;
//
//    while (i < sizeof(sVS1053b_Realtime_MIDI_Plugin) / sizeof(sVS1053b_Realtime_MIDI_Plugin[0])) {
//        unsigned short addr, n, val;
//        addr = sVS1053b_Realtime_MIDI_Plugin[i++];
//        n = sVS1053b_Realtime_MIDI_Plugin[i++];
//        while (n--) {
//            val = sVS1053b_Realtime_MIDI_Plugin[i++];
//            WriteSCIRegister(addr, val);
//        }
//    }
//}

SPISettings vsSettings(245000, MSBFIRST, SPI_MODE0);

VS1053MIDI::VS1053MIDI(byte CS, byte XCS, byte XRESET, byte XDCS, byte DREQ, byte RTL)
{
	cs=CS;
	xcs=XCS;
	xreset=XRESET;
	xdcs=XDCS;
	dreq=DREQ;
	rtl=RTL;

	pinMode(CS, OUTPUT);
    pinMode(XCS, OUTPUT);
    pinMode(XRESET, OUTPUT);
    pinMode(XDCS, OUTPUT);
    pinMode(RTL, OUTPUT);
    pinMode(DREQ, INPUT);

    digitalWrite(CS, 1);
    digitalWrite(XCS, 1);
    digitalWrite(XDCS, 1);
    digitalWrite(XRESET, 0);
    digitalWrite(RTL, 1);

    SPI.begin();
    SPI.usingInterrupt(255);

    delay(10);

    digitalWrite(XRESET, 1);

    delay(10);
}

void VS1053MIDI::NoteOn(byte Channel, byte Note, byte Velocity)
{
    SendMIDICommand(MIDI_CMD_NOTEON | Channel, Note, Velocity);
}
void VS1053MIDI::NoteOff(byte Channel, byte Note, byte Velocity)
{
    SendMIDICommand(MIDI_CMD_NOTEOFF | Channel, Note, Velocity);
}
void VS1053MIDI::Aftertouch(byte Channel, byte Note, byte Touch)
{
    SendMIDICommand(MIDI_CMD_AFT | Channel, Note, Touch);
}
void VS1053MIDI::Patch(byte Channel, byte Patch)
{
    SendMIDICommand(MIDI_CMD_PATCH | Channel, Patch);
}
void VS1053MIDI::Pressure(byte Channel, byte Pressure)
{
    SendMIDICommand(MIDI_CMD_PRESS | Channel, Pressure);
}
void VS1053MIDI::Pitch(byte Channel, word Pitch)
{
    SendMIDICommand(MIDI_CMD_PRESS | Channel, Pitch & 0x7F, (Pitch >> 7) & 0x7F);
}

void VS1053MIDI::ModulationController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_MOD, Value);
}
void VS1053MIDI::VolumeController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_VOL, Value);
}
void VS1053MIDI::BreathController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_BREATH, Value);
}
void VS1053MIDI::PanController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_PAN, Value);
}
void VS1053MIDI::ExpressionController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_EXPR, Value);
}
void VS1053MIDI::PortamentoController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_PORTA, Value);
}
void VS1053MIDI::SustainedController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_SUST, Value);
}
void VS1053MIDI::ResonanceController(byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, MIDI_CC_RESO, Value);
}

void VS1053MIDI::OtherController(byte Controller, byte Channel, byte Value)
{
    SendMIDICommand(MIDI_CMD_CC | Channel, Controller, Value);
}

void VS1053MIDI::EarSpace(bool On) 
{
    word currentMode = ReadSCIRegister(SCI_MODE);

    if (On)
        currentMode |= 128;
    else
        currentMode &= ~((word)128);
}
void VS1053MIDI::Volume(byte Left, byte Right) 
{
    WriteSCIRegister(SCI_VOL, (((word)Left) << 8) | ((word)Right));
}

void VS1053MIDI::SendMIDICommand(byte Command, byte Param1)
{
    while (!digitalRead(dreq));

    digitalWrite(xdcs, 0);

    SPI.beginTransaction(vsSettings);

    SPI.transfer16(Command);
    SPI.transfer16(Param1);

    SPI.endTransaction();

    digitalWrite(xdcs, 1);
}
void VS1053MIDI::SendMIDICommand(byte Command, byte Param1, byte Param2)
{
    while (!digitalRead(dreq));

    digitalWrite(xdcs, 0);

    SPI.beginTransaction(vsSettings);

    SPI.transfer16(Command);
    SPI.transfer16(Param1);
    SPI.transfer16(Param2);

    SPI.endTransaction();

    digitalWrite(xdcs, 1);
}

word VS1053MIDI::ReadSCIRegister(byte Register)
{

    word value;

    digitalWrite(xcs, 0);
    SPI.beginTransaction(vsSettings);
    SPI.transfer(0x03);
    SPI.transfer(Register);
    value = SPI.transfer16(0xFFFF);
    SPI.endTransaction();
    digitalWrite(xcs, 1);

    return value;
}
void VS1053MIDI::WriteSCIRegister(byte Register, word Value)
{
    digitalWrite(xcs, 0);
    SPI.beginTransaction(vsSettings);
    SPI.transfer(0x02);
    SPI.transfer(Register);
    SPI.transfer16(Value);
    SPI.endTransaction();
    digitalWrite(xcs, 1);
}

void VS1053MIDI::Reset()
{
    digitalWrite(xreset, 0);
    delay(10);
    digitalWrite(xreset, 1);
    delay(10);
}