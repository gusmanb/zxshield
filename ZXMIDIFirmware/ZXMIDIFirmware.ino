/*
 Name:		ZXMIDIFirmware.ino
 Created:	05/03/2020 16:31:51
 Author:	geniw
*/

#include "Arduino.h"
#include <SPI.h>
#include "VS1053MIDI.h"
#include "ZXShield.h"
#include "ZXPeripheral.h"

#define CS 53
#define XCS 8
#define XRESET 9
#define XDCS 10
#define DREQ 11
#define RTL 12

#define NO_INTERRUPTS_ON_COMMS

typedef struct _playingNote
{
    volatile bool playing;
    volatile byte note;
    volatile byte sixteenthsLeft;

}playingNote, *PplayingNote;

typedef struct _groupedChannel
{
    volatile byte channelNumber;
    volatile char octaveOffset;

}groupedChannel, *PGroupedChannel;

typedef struct _channelGroup
{
    volatile byte channelCount;
    volatile groupedChannel channels[10];

}channelGroup, *PchannelGroup;

volatile PREGISTER regData[16];
volatile PREGISTER regError;

VS1053MIDI* midi;
volatile byte mode;
volatile byte currentSequence;
volatile byte posInSequence;
volatile byte bpm;
volatile float beatFreq;
volatile byte currentChannel;
volatile byte currentOctave;
volatile byte noteDuration;
volatile byte offset;
volatile byte currentOnVelocity;
volatile byte currentOffVelocity;

volatile playingNote channelNotes[16];

volatile channelGroup groups[10];
volatile byte currentGroup = 0;
volatile bool groupEnabled = false;

//volatile bool exec = false;

#define OCTAVE_LENGTH 12

typedef enum _NOTES
{
    NOTE_C = 0,
    NOTE_D = 1,
    NOTE_E = 2,
    NOTE_F = 3,
    NOTE_G = 4,
    NOTE_A = 5,
    NOTE_B = 6,
};

const byte noteNumbers[] = { 33, 35, 24, 26, 28, 29, 31 }; //abcdefg

volatile int tId;

// the setup function runs once when you press reset or power the board
void setup() 
{

    Serial.begin(115200);

    memset((void*)channelNotes, 0, sizeof(playingNote) * 16);
    memset((void*)regData, 0, sizeof(PREGISTER) * 16);
    groupEnabled = false;
    currentGroup = 0;
    bpm = 60;
    calculateCMR();
    noteDuration = 16;

    midi = new  VS1053MIDI(CS, XCS, XRESET, XDCS, DREQ, RTL);

    ZXShield::Initialize();
    
    /*
    Registers:
     0-15 buffers
          Command buffer in direct mode, used randomly by the user
          Partiture buffer in partiture mode, used sequentially by the user
     16   Mode selector. When mode is changed the MIDI system gets reset
     17   BPM selector
     18   Partiture mode last error
     19   Partiture mode current register
    */


    for (int buc = 0; buc < 16; buc++)
    {
        regData[buc] = NULL;
        ZXShield::Peripheral.CreateRegister(buc, STREAM_R, true, true, NULL, NULL, NULL, NULL, bufferWritten);
    }

    ZXShield::Peripheral.CreateRegister(16, BYTE_R, true, true, NULL, NULL, NULL, NULL, changeMode);
    ZXShield::Peripheral.CreateRegister(17, BYTE_R, true, true, NULL, NULL, NULL, NULL, changeBPM);
    ZXShield::Peripheral.CreateRegister(18, BYTE_R, true, false, NULL, NULL, NULL, errorRead);
    ZXShield::Peripheral.CreateRegister(19, BYTE_R, true, false, NULL, currentRegRead);
    ZXShield::EnablePeripheral();

    ZXShield::ResetCPU();
    
    //midi->Patch(0, 97);
    midi->NoteOff(0, 85, 120);
    midi->Patch(0, 8);
    midi->Patch(1, 61);
    midi->VolumeController(0, 127);
    midi->VolumeController(1, 127);
    delay(1000);
    midi->NoteOn(0, 73, 127);
    midi->NoteOn(1, 73, 127);
    delay(200);
    midi->NoteOff(0, 73, 120);
    midi->NoteOff(1, 73, 120);
    midi->NoteOn(0, 73, 127);
    midi->NoteOn(1, 73, 127);
    delay(1000);
    midi->NoteOff(0, 73, 120);
    midi->NoteOff(1, 73, 120);
    delay(500);
    midi->Patch(0, 0);
    midi->Patch(1, 0);

}

void bufferWritten(PREGISTER Register)
{
    ZXPeripheral::LockRegister(Register);
    regData[ZXShield::Peripheral.CurrentRegisterId()] = Register;
}

void currentRegRead(PREGISTER Register)
{
    ZXPeripheral::SetByteValue(Register, currentSequence);
}

void errorRead(PREGISTER Register)
{
    ZXPeripheral::SetByteValue(Register, 0);
}

void changeMode(PREGISTER Register)
{
    ZXPeripheral::LockRegister(Register);
    mode = ZXPeripheral::GetByteValue(Register);

    disablerTimer();

    if (mode)
    {
        for (int buc = 0; buc < 16; buc++)

        {
            PREGISTER reg = ZXShield::Peripheral.GetRegister(buc);
            ZXPeripheral::SetStreamValue(reg, "                                                                \0");
            ZXPeripheral::UnlockRegister(reg);
            regData[buc] = NULL;
            channelNotes[buc].playing = false;
        }

        currentSequence = 0;
        posInSequence = 0;
        bpm = 60;
        currentChannel = 0;
        offset = 0;
        currentOctave = 4;
        currentOnVelocity = 127;
        currentOffVelocity = 64;
        noteDuration = 16;
        groupEnabled = false;
        currentGroup = 0;

        SPI.usingInterrupt(255);

        midi->Reset();
        enableTimer();

    }
    else
    {
        for (int buc = 0; buc < 16; buc++)
        {
            PREGISTER reg = ZXShield::Peripheral.GetRegister(buc);
            ZXPeripheral::SetStreamValue(reg, "    \0");
            ZXPeripheral::UnlockRegister(reg);
            regData[buc] = NULL;
            channelNotes[buc].playing = false;
        }


        SPI.notUsingInterrupt(255);
        midi->Reset();
    }

    ZXPeripheral::UnlockRegister(Register);
}

void changeBPM(PREGISTER Register)
{
    ZXPeripheral::LockRegister(Register);
    bpm = ZXPeripheral::GetByteValue(Register);
    calculateCMR();

}

void enableTimer()
{
    //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1 = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = calculateCMR();
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS11 and CS10 (prescaler 64)
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

}

void disablerTimer()
{
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1 = 0;//initialize counter value to 0
    OCR1A = 0;
    TIMSK1 &= ~(1 << OCIE1A);
}

word calculateCMR()
{
    beatFreq = 1.0 / (60.0 / (float)bpm);

    //WriteString("BF");
    //WriteNumber(beatFreq);
    word divider = (unsigned long)64 * ((unsigned long)(beatFreq * 4));
    //WriteNumber(divider);
    unsigned long CRM = ((unsigned long)16000000 / divider) - 1;
    //WriteString("CRM");
    //WriteNumber((word)CRM);
    return (word)CRM;
}

ISR(TIMER1_COMPA_vect) 
{
    playStream();
}

void playStream()
{
    for (word buc = 0; buc < 16; buc++)
    {
        if (channelNotes[buc].playing)
        {
            channelNotes[buc].sixteenthsLeft--;
            if (channelNotes[buc].sixteenthsLeft == 0)
            {
                channelNotes[buc].playing = false;
                
                if(channelNotes[buc].note != 0)
                    midi->NoteOff(buc, channelNotes[buc].note, currentOffVelocity);
            }
        }
    }

    if (regData[currentSequence] != NULL)
    {

        PREGISTER reg = regData[currentSequence];

        PSTREAM_REGISTER sr = (PSTREAM_REGISTER)reg->Data;

        bool canExecute = true;
        bool sequenceError = false;

        //WriteString("Register in interrupt");

        while (canExecute)
        {
            //WriteString("Action");
            //WriteSegment((volatile byte*)&sr->Buffer[posInSequence], 1);
            //WriteString("");

            byte value = sr->Buffer[posInSequence];

            if (value >= '1' && value <= '5')
            {
                //WriteString("Note duration");

                noteDuration = 1 << (sr->Buffer[posInSequence] - '1');
                posInSequence++;
            }
            else if (value >= 'a' && value <= 'g')
            {
                //WriteString("Note lower");

                if (groupEnabled)
                {
                    //WriteString("Playing group");
                    //WriteNumber(currentGroup);

                    volatile channelGroup* grp = &groups[currentGroup];

                    for (int buc = 0; buc < grp->channelCount; buc++)
                    {
                        if (channelNotes[buc].playing)
                        {
                            //WriteString("Group bussy");
                            canExecute = false;
                            break;
                        }
                    }

                    if (canExecute)
                    {

                        for (int buc = 0; buc < grp->channelCount; buc++)
                        {
                            //WriteString("Channel group");
                            //WriteNumber(buc);
                            //WriteString("Channel n.");
                            //WriteNumber(buc);
                            byte noteH = ((currentOctave + grp->channels[buc].octaveOffset) - 1) * OCTAVE_LENGTH + noteNumbers[sr->Buffer[posInSequence] - 'a'] + offset;

                            volatile playingNote* note = &channelNotes[grp->channels[buc].channelNumber];

                            note->playing = true;
                            note->note = noteH;
                            note->sixteenthsLeft = noteDuration;

                            midi->NoteOn(grp->channels[buc].channelNumber, noteH, currentOnVelocity);
                        }

                        offset = 0;
                        posInSequence++;
                    }
                }
                else
                {

                    if (channelNotes[currentChannel].playing)
                    {
                        //WriteString("Can't execute");
                        canExecute = false;
                    }
                    else
                    {
                        byte noteH = (currentOctave - 1) * OCTAVE_LENGTH + noteNumbers[sr->Buffer[posInSequence] - 'a'] + offset;
                        offset = 0;

                        channelNotes[currentChannel].playing = true;
                        channelNotes[currentChannel].note = noteH;
                        channelNotes[currentChannel].sixteenthsLeft = noteDuration;

                        midi->NoteOn(currentChannel, noteH, 127);

                        posInSequence++;
                    }
                }
            }
            else if (value >= 'A' && value <= 'G')
            {
                //WriteString("Note lower");

                if (groupEnabled)
                {
                    volatile channelGroup* grp = &groups[currentGroup];

                    for (int buc = 0; buc < grp->channelCount; buc++)
                    {
                        if (channelNotes[buc].playing)
                        {
                            canExecute = false;
                            break;
                        }
                    }

                    if (canExecute)
                    {

                        for (int buc = 0; buc < grp->channelCount; buc++)
                        {
                            byte noteH = (currentOctave + grp->channels[buc].octaveOffset) * OCTAVE_LENGTH + noteNumbers[sr->Buffer[posInSequence] - 'A'] + offset;

                            volatile playingNote* note = &channelNotes[grp->channels[buc].channelNumber];

                            note->playing = true;
                            note->note = noteH;
                            note->sixteenthsLeft = noteDuration;

                            midi->NoteOn(grp->channels[buc].channelNumber, noteH, currentOnVelocity);
                        }

                        offset = 0;
                        posInSequence++;
                    }
                }
                else
                {

                    if (channelNotes[currentChannel].playing)
                    {
                        //WriteString("Can't execute");
                        canExecute = false;
                    }
                    else
                    {
                        byte noteH = currentOctave * OCTAVE_LENGTH + noteNumbers[sr->Buffer[posInSequence] - 'A'] + offset;
                        offset = 0;

                        channelNotes[currentChannel].playing = true;
                        channelNotes[currentChannel].note = noteH;
                        channelNotes[currentChannel].sixteenthsLeft = noteDuration;

                        midi->NoteOn(currentChannel, noteH, 127);

                        posInSequence++;
                    }
                }
            }
            else if (value == '.')
            {
                if (groupEnabled)
                {
                    volatile channelGroup* grp = &groups[currentGroup];

                    for (int buc = 0; buc < grp->channelCount; buc++)
                    {
                        if (channelNotes[buc].playing)
                        {
                            canExecute = false;
                            break;
                        }
                    }

                    if (canExecute)
                    {

                        for (int buc = 0; buc < grp->channelCount; buc++)
                        {
                            volatile playingNote* note = &channelNotes[grp->channels[buc].channelNumber];

                            note->playing = true;
                            note->note = 0;
                            note->sixteenthsLeft = noteDuration;
                        }

                        offset = 0;
                        posInSequence++;
                    }
                }
                else
                {

                    if (channelNotes[currentChannel].playing)
                    {
                        //WriteString("Can't execute");
                        canExecute = false;
                    }
                    else
                    {
                        offset = 0;

                        channelNotes[currentChannel].playing = true;
                        channelNotes[currentChannel].note = 0;
                        channelNotes[currentChannel].sixteenthsLeft = noteDuration;

                        posInSequence++;
                    }
                }
            }
            else if (value == '#')
            {
                //WriteString("Sharp");
                offset = 1;
                posInSequence++;
            }
            else if (value == '$')
            {
                //WriteString("Flat");
                offset = -1;
                posInSequence++;

            }
            else if (value == 'M')
            {
                //WriteString("Channel");

                if (posInSequence + 2 >= sr->BufferLength)
                {
                    ZXPeripheral::SetByteValue(regError, 1);
                    //WriteString("Buffer too short");
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    byte channel = (sr->Buffer[posInSequence + 1] - '0') * 10 + (sr->Buffer[posInSequence + 2] - '0');

                    if (channel < 16)
                        currentChannel = channel;

                    groupEnabled = false;
                    posInSequence += 3;
                }
            }
            else if (value == 'N')
            {
                //Enable group
                //WriteString("Octave");
                if (posInSequence + 1 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 7);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    currentGroup = (sr->Buffer[posInSequence + 1] - '0');
                    groupEnabled = true;
                    posInSequence += 2;
                    //WriteString("Group enabled");
                    //WriteNumber(currentGroup);
                }
            }
            else if (value == 'O')
            {
                //WriteString("Octave");
                if (posInSequence + 1 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 3);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    currentOctave = (sr->Buffer[posInSequence + 1] - '0');
                    posInSequence += 2;
                }
            }
            else if (value == 'P')
            {
                //WriteString("Patch");

                if (posInSequence + 3 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 2);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    byte patch = (sr->Buffer[posInSequence + 1] - '0') * 100 + (sr->Buffer[posInSequence + 2] - '0') * 10 + (sr->Buffer[posInSequence + 3] - '0');

                    if (patch < 128)
                    {
                        if (groupEnabled)
                        {
                            volatile channelGroup* grp = &groups[currentChannel];

                            for (int buc = 0; buc < grp->channelCount; buc++)
                                midi->Patch(grp->channels[buc].channelNumber, patch);
                        }
                        else
                            midi->Patch(currentChannel, patch);

                        
                    }
                    posInSequence += 4;
                }
            }
            else if (value == 'V')
            {
                //WriteString("Volume");
                if (posInSequence + 3 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 4);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    byte vol = (sr->Buffer[posInSequence + 1] - '0') * 100 + (sr->Buffer[posInSequence + 2] - '0') * 10 + (sr->Buffer[posInSequence + 3] - '0');

                    if (vol < 128)
                    {
                        if (groupEnabled)
                        {
                            volatile channelGroup *grp = &groups[currentChannel];

                            for(int buc = 0; buc < grp->channelCount; buc++)
                                midi->VolumeController(grp->channels[buc].channelNumber, vol);
                        }
                        else
                            midi->VolumeController(currentChannel, vol);
                    }
                    posInSequence += 4;
                }

            }
            else if (value == '@')
            {
                //WriteString("Volume");
                if (posInSequence + 5 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 4);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    byte cont = (sr->Buffer[posInSequence + 1] - '0') * 10 + (sr->Buffer[posInSequence + 2] - '0');
                    byte val = (sr->Buffer[posInSequence + 3] - '0') * 100 + (sr->Buffer[posInSequence + 4] - '0') * 10 + (sr->Buffer[posInSequence + 5] - '0');

                    if (val < 128)
                    {
                        if (groupEnabled)
                        {
                            volatile channelGroup* grp = &groups[currentChannel];

                            for (int buc = 0; buc < grp->channelCount; buc++)
                                midi->OtherController(grp->channels[buc].channelNumber, cont, val);
                        }
                        else
                            midi->OtherController(currentChannel, cont, val);
                    }
                    posInSequence += 6;
                }
            }
            else if (value == 'T')
            {
                //WriteString("Tempo");

                if (posInSequence + 3 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 5);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    bpm = (sr->Buffer[posInSequence + 1] - '0') * 100 + (sr->Buffer[posInSequence + 2] - '0') * 10 + (sr->Buffer[posInSequence + 3] - '0');

                    //WriteNumber(bpm);

                    disablerTimer();
                    enableTimer();

                    posInSequence += 4;
                }
            }
            else if (value == '>') 
            {
                if (posInSequence + 3 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 4);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    byte offv = (sr->Buffer[posInSequence + 1] - '0') * 100 + (sr->Buffer[posInSequence + 2] - '0') * 10 + (sr->Buffer[posInSequence + 3] - '0');

                    if (offv < 128)
                        currentOffVelocity = offv;

                    posInSequence += 4;
                }
            }
            else if (value == '<') 
            {
                if (posInSequence + 3 >= sr->BufferLength)
                {
                    //WriteString("Buffer too short");
                    ZXPeripheral::SetByteValue(regError, 4);
                    sequenceError = true;
                    canExecute = false;
                }
                else
                {
                    byte onv = (sr->Buffer[posInSequence + 1] - '0') * 100 + (sr->Buffer[posInSequence + 2] - '0') * 10 + (sr->Buffer[posInSequence + 3] - '0');

                    if (onv < 128)
                        currentOnVelocity = onv;

                    posInSequence += 4;
                }
            }
            else if (value == 'n') //Configure group
            {
                //n(num)(gr0_chan)(gr0_o_offset)...(grn_chan)(grn_o_offset)

                byte groupNum;
                byte chanCount;

                if (posInSequence + 2 >= sr->BufferLength)
                {
                    ZXPeripheral::SetByteValue(regError, 6);
                    sequenceError = true;
                    canExecute = false;
                }

                groupNum = (sr->Buffer[posInSequence + 1] - '0');
                chanCount = (sr->Buffer[posInSequence + 2] - '0');

                if (posInSequence + 2 + (chanCount * 3) >= sr->BufferLength)
                {
                    ZXPeripheral::SetByteValue(regError, 6);
                    sequenceError = true;
                    canExecute = false;
                }

                volatile channelGroup* currentGroup = &groups[groupNum];
                posInSequence += 3;

                currentGroup->channelCount = chanCount;

                for (int buc = 0; buc < chanCount; buc++)
                {
                    currentGroup->channels[buc].channelNumber = (sr->Buffer[posInSequence] - '0') * 10 + (sr->Buffer[posInSequence + 1] - '0');
                    currentGroup->channels[buc].octaveOffset = sr->Buffer[posInSequence + 2] == '+' ? 1 : sr->Buffer[posInSequence + 2] == '-' ? -1 : 0;

                    posInSequence += 3;
                }

            }
            else if (value == 'R')
            {
                for (int buc = 0; buc < 16; buc++)
                {
                    PREGISTER reg = ZXShield::Peripheral.GetRegister(buc);
                    ZXPeripheral::SetStreamValue(reg, "                                                                \0");
                    ZXPeripheral::UnlockRegister(reg);
                    regData[buc] = NULL;
                    channelNotes[buc].playing = false;
                }

                currentSequence = 0;
                posInSequence = 0;
                bpm = 60;
                currentChannel = 0;
                offset = 0;
                currentOctave = 4;
                currentOnVelocity = 127;
                currentOffVelocity = 64;
                noteDuration = 16;
                groupEnabled = false;
                currentGroup = 0;

                disablerTimer();
                enableTimer();

                canExecute = false;
            }
            else
            {
                //WriteString("Unknown");
                ZXPeripheral::SetByteValue(regError, 255);
                //WriteNumber(sr->Buffer[posInSequence] == 'T');
                sequenceError = true;

            }

            if (sequenceError)
            {
                //WriteString("Sequence error");
                for (int buc = 0; buc < 16; buc++)
                {
                    PREGISTER reg = ZXShield::Peripheral.GetRegister(buc);
                    ZXPeripheral::SetStreamValue(reg, "                                                                \0");
                    ZXPeripheral::UnlockRegister(reg);
                    regData[buc] = NULL;
                    channelNotes[buc].playing = false;
                }

                currentSequence = 0;
                posInSequence = 0;
                bpm = 60;
                currentChannel = 0;
                offset = 0;
                currentOctave = 4;
                currentOnVelocity = 127;
                currentOffVelocity = 64;
                noteDuration = 16;
                groupEnabled = false;
                currentGroup = 0;

                disablerTimer();
                enableTimer();

                canExecute = false;

                //WriteString("Reset");
            }
            else
            {
                //WriteString("Test end");
                if (posInSequence >= sr->BufferLength)
                {
                    //WriteString("Finished");
                    ZXPeripheral::UnlockRegister(reg);
                    regData[currentSequence] = NULL;

                    currentSequence++;
                    posInSequence = 0;

                    if (currentSequence > 15)
                        currentSequence = 0;

                   // WriteString("Next sequence");
                    //WriteNumber(currentSequence);

                    reg = regData[currentSequence];

                    if (reg == NULL)
                    {
                        canExecute = false;
                       // WriteString("Next sequence");
                    }
                    else
                        sr = (PSTREAM_REGISTER)reg->Data;
                    
                }
            }
        }
    }
}

// the loop function runs over and over again until power down or reset
void loop() 
{
    if (!mode)
    {
        for (int buc = 0; buc < 16; buc++)
        {
            if (regData[buc] != NULL)
            {
                PREGISTER reg = regData[buc];
                PSTREAM_REGISTER sr = (PSTREAM_REGISTER)reg->Data;

                if (sr->BufferLength == 4)
                {
                    switch (sr->Buffer[0])
                    {
                    case 0: //note on
                        midi->NoteOn(sr->Buffer[1], sr->Buffer[2], sr->Buffer[3]);
                        break;
                    case 1: //note off
                        midi->NoteOff(sr->Buffer[1], sr->Buffer[2], sr->Buffer[3]);
                        break;
                    case 2: //touch
                        midi->Aftertouch(sr->Buffer[1], sr->Buffer[2], sr->Buffer[3]);
                        break;
                    case 3: //patch
                        midi->Patch(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 4: //pressure
                        midi->Pressure(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 5: //pitch
                        midi->Pitch(sr->Buffer[1], (word)sr->Buffer[2] | ((word)sr->Buffer[3] << 8));
                        break;
                    case 6: //modulation
                        midi->ModulationController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 7: //volume
                        midi->VolumeController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 8: //breath
                        midi->BreathController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 9: //pan
                        midi->PanController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 10: //expression
                        midi->ExpressionController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 11: //portamento
                        midi->PortamentoController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 12: //sustained
                        midi->SustainedController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 13: //resonance
                        midi->ResonanceController(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    case 15: //other controller
                        midi->OtherController(sr->Buffer[1], sr->Buffer[2], sr->Buffer[3]);
                        break;
                    case 16: //general volume
                        midi->Volume(sr->Buffer[1], sr->Buffer[2]);
                        break;
                    }
                }

                regData[buc] = NULL;
                ZXPeripheral::UnlockRegister(reg);
            }
        }
    }
}
//
//#pragma region Serial functions
//
//byte serialCheckRxComplete()
//{
//    return(UCSR0A & _BV(RXC0));		// nonzero if serial data is available to read.
//}
//byte serialCheckTxReady()
//{
//    return(UCSR0A & _BV(UDRE0));		// nonzero if transmit register is ready to receive new data.
//}
//
//void WriteNumber(word Value)
//{
//    char buffer[32];
//    memset(buffer, 0, 32);
//    ultoa(Value, buffer, 10);
//    WriteString(buffer);
//}
//void WriteString(const char* String)
//{
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    noInterrupts();
//#endif
//
//    int pos = 0;
//    int len = strlen(String);
//
//    while (pos < len)
//    {
//        while (!serialCheckTxReady())
//        {
//            ;;
//        }
//
//        UDR0 = String[pos++];
//    }
//
//    while (!serialCheckTxReady())
//    {
//        ;;
//    }
//
//    UDR0 = '\n';
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    interrupts();
//#endif
//
//}
//void WriteSegment(volatile byte* volatile target, word length)
//{
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    noInterrupts();
//#endif
//
//    int pos = 0;
//
//    while (pos < length)
//    {
//        while (!serialCheckTxReady())
//        {
//            ;;
//        }
//
//        UDR0 = target[pos++];
//    }
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    interrupts();
//#endif
//
//}
//
//byte ReadByte()
//{
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    noInterrupts();
//#endif
//
//    while (!serialCheckRxComplete())
//    {
//        ;;
//    }
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    interrupts();
//#endif
//
//    return UDR0;
//}
//unsigned int ReadInt()
//{
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    noInterrupts();
//#endif
//
//    unsigned int value;
//
//    while (!serialCheckRxComplete())
//    {
//        ;;
//    }
//
//    value = UDR0;
//
//    while (!serialCheckRxComplete())
//    {
//        ;;
//    }
//
//    value |= UDR0 << 8;
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    interrupts();
//#endif
//
//    return value;
//
//}
//void ReadSegment(volatile byte* target, word length)
//{
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    noInterrupts();
//#endif
//
//    while (length-- > 0)
//    {
//
//        while (!serialCheckRxComplete())
//        {
//            ;;
//        }
//
//        *target = (byte)UDR0;
//        target++;
//
//    }
//
//#ifdef NO_INTERRUPTS_ON_COMMS
//    interrupts();
//#endif
//
//}
//
//#pragma endregion
