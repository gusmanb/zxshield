/*
 Name:		ZXSerialLoaderFirmware.ino
 Created:	07/02/2020 1:23:08
 Author:	geniw
*/

#include "arduino.h"
#include "ZXShield.h"
#include "SNALoader.h"
#include "HEXLoader.h"
#include <avr/io.h> 

#define NO_INTERRUPTS_ON_COMMS

volatile byte* volatile header;			//pointer to SNA header in the virtual RAM
volatile byte* volatile status;			//Pointer to status in the virtual RAM
volatile word* volatile startAddress;	//Pointer to start address in virtual RAM
volatile word* volatile segmentDest;	//Pointer to segment destination in virtual RAM
volatile word* volatile segmentSize;	//Pointer to segment size in virtual RAM
volatile byte* volatile virtualSegment;	//Pointer to the segment in virtual RAM

volatile byte virtualRAM[4096];			//Virtual RAM buffer

volatile bool vramDisabled = false;

volatile word exitAddress;

// the setup function runs once when you press reset or power the board
void setup() 
{
	//Clear virtual RAM
	memset((void*)virtualRAM, 0, 4096);

	//Initialize the shield and vRAM handler
	ZXShield::Initialize();
	ZXShield::ROM.SetROMHandler(RAMHandler);

	//Reset CPU (for sanity)
	ZXShield::ResetCPU();

	//Initialize serial port
	Serial.begin(1000000);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	byte op = ReadByte();

	if (op == 'H')	//HEX loader
	{
		//Create pointers to special virtual RAM addresses
		status = &virtualRAM[HEX_STATUS_ADDRESS];
		startAddress = (volatile word* volatile)&virtualRAM[HEX_START_ADDRESS_ADDRESS];
		segmentDest = (volatile word* volatile)&virtualRAM[HEX_SEGMENT_DEST_ADDRESS];
		segmentSize = (volatile word* volatile)&virtualRAM[HEX_SEGMENT_SIZE_ADDRESS];
		virtualSegment = &virtualRAM[HEX_SEGMENT_ADDRESS];
		exitAddress = HEX_EXIT_ADDRESS;

		//Copy program loader
		memset((void*)virtualRAM, 0, 4096);
		memcpy_P((void*)virtualRAM, HEXProgram, HEX_SEGMENT_ADDRESS);

		//Notify we're ready
		WriteString("RDY");
		loadHEX();
	}
	else if (op == 'S')	//SNA loader
	{
		//Create pointers to special virtual RAM addresses
		header = &virtualRAM[SNA_HEADER_ADDRESS];
		status = &virtualRAM[SNA_STATUS_ADDRESS];
		startAddress = (volatile word* volatile)&virtualRAM[SNA_START_ADDRESS_ADDRESS];
		segmentDest = (volatile word* volatile)&virtualRAM[SNA_SEGMENT_DEST_ADDRESS];
		segmentSize = (volatile word* volatile)&virtualRAM[SNA_SEGMENT_SIZE_ADDRESS];
		virtualSegment = &virtualRAM[SNA_SEGMENT_ADDRESS];
		exitAddress = SNA_EXIT_ADDRESS;

		//Copy program loader
		memset((void*)virtualRAM, 0, 4096);
		memcpy_P((void*)virtualRAM, SNAProgram, SNA_SEGMENT_ADDRESS);

		//Notify we're ready
		WriteString("RDY");
		loadSNA();
	}
}

void loadSNA()
{
	bool finished = false;
	vramDisabled = false;

	WriteString("RDY");
	//Read the SNA header
	ReadSegment(header, SNA_HEADER_SIZE);

	//read first segment data
	byte isLast = ReadByte();
	//The first segment address is also the program start address
	*startAddress = *segmentDest = ReadInt();
	*segmentSize = ReadInt();
	//Acknowledge the data
	WriteString("OK");
	//Receive the segment
	ReadSegment(virtualSegment, *segmentSize);

	//
	if (isLast)
	{
		*status = 0xAA;		//Total transfer is less or equal to a segment
							//As the program will read always one segment and
							//then check the status we can set the finish flag
							//in the first transfer
		finished = true;
	}
	else
		*status = 0x00;		//clear status

	vramDisabled = false;
	//Start the transfer by enabling the vRAM and raising a NMI on the speccy
	ZXShield::EnableROMWithNMI();

	//transfer loop
	while (!finished)
	{
		//Wait for the segment transfer
		while (*status != 1);

		//Now the program is on the wait loop, request the next segment
		WriteString("NEXT");

		//Read segment data
		isLast = ReadByte();
		*startAddress = *segmentDest = ReadInt();
		*segmentSize = ReadInt();

		//Acknowledge the data
		WriteString("OK");

		//Receive the segment
		ReadSegment(virtualSegment, *segmentSize);

		if (isLast)	//Was this the last segment?
		{
			finished = true;

			*status = 0x55;			//Signal the program that it has to copy a segment
			while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
			while (*status != 1);	//Wait until the program returns to the wait loop
			*status = 0xAA;			//Signal the program that we have finished
		}
		else
		{
			*status = 0x55;
			while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
		}
	}

	//Inform the client we're waiting the program to write the last segment
	WriteString("WAIT_LAST");

	//Wait until the program has read the last instruction
	while (!vramDisabled);

	//Notify to the client the succes transfer
	WriteString("TRANSFER_SUCCESS");
}

void loadHEX()
{
	WriteString("RDY");

	bool finished = false;
	vramDisabled = false;

	//read first segment data
	byte isLast = ReadByte();
	//The first segment address is also the program start address
	*startAddress = *segmentDest = ReadInt();
	*segmentSize = ReadInt();
	//Acknowledge the data
	WriteString("OK");
	//Receive the segment
	ReadSegment(virtualSegment, *segmentSize);

	//
	if (isLast)
	{
		*status = 0xAA;		//Total transfer is less or equal to a segment
							//As the program will read always one segment and
							//then check the status we can set the finish flag
							//in the first transfer
		finished = true;
	}
	else
		*status = 0x00;		//clear status

	vramDisabled = false;
	//Start the transfer by enabling the vRAM and raising a NMI on the speccy
	ZXShield::EnableROMWithNMI();

	//transfer loop
	while (!finished)
	{
		//Wait for the segment transfer
		while (*status != 1);

		//Now the program is on the wait loop, request the next segment
		WriteString("NEXT");

		//Read segment data
		isLast = ReadByte();
		*startAddress = *segmentDest = ReadInt();
		*segmentSize = ReadInt();

		//Acknowledge the data
		WriteString("OK");

		//Receive the segment
		ReadSegment(virtualSegment, *segmentSize);

		if (isLast)	//Was this the last segment?
		{
			finished = true;

			*status = 0x55;			//Signal the program that it has to copy a segment
			while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
			while (*status != 1);	//Wait until the program returns to the wait loop
			*status = 0xAA;			//Signal the program that we have finished
		}
		else
		{
			*status = 0x55;
			while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
		}
	}

	//Inform the client we're waiting the program to write the last segment
	WriteString("WAIT_LAST");

	//Wait until the program has read the last instruction
	while (!vramDisabled);

	//Notify to the client the succes transfer
	WriteString("TRANSFER_SUCCESS");
}


void RAMHandler(word Address, byte Operation)
{
	//Write?
	if (Operation)
		virtualRAM[Address] = ZXShield::InputByte();//Store data
	else
	{
		ZXShield::OutputROMByte(virtualRAM[Address]); //send data

		if (Address == exitAddress) //Did the speccy read the last instruction?
		{
			//disable vRAM
			vramDisabled = true;
			ZXShield::DisableROM();
		}

	}
}

#pragma region Serial functions

byte serialCheckRxComplete()
{
	return(UCSR0A & _BV(RXC0));		// nonzero if serial data is available to read.
}
byte serialCheckTxReady()
{
	return(UCSR0A & _BV(UDRE0));		// nonzero if transmit register is ready to receive new data.
}

void WriteNumber(word Value)
{
	char buffer[32];
	memset(buffer, 0, 32);
	ultoa(Value, buffer, 10);
	WriteString(buffer);
}
void WriteString(const char* String)
{

#ifdef NO_INTERRUPTS_ON_COMMS
	noInterrupts();
#endif

	int pos = 0;
	int len = strlen(String);

	while (pos < len)
	{
		while (!serialCheckTxReady())
		{
			;;
		}

		UDR0 = String[pos++];
}
	while (!serialCheckTxReady())
	{
		;;
	}

	UDR0 = '\n';

#ifdef NO_INTERRUPTS_ON_COMMS
	interrupts();
#endif

}
void WriteSegment(volatile byte* target, word length)
{

#ifdef NO_INTERRUPTS_ON_COMMS
	noInterrupts();
#endif

	while (length-- > 0)
	{

		while (!serialCheckTxReady())
		{
			;;
		}

		UDR0 = *target;
		target++;

	}

#ifdef NO_INTERRUPTS_ON_COMMS
	interrupts();
#endif

}

byte ReadByte()
{

#ifdef NO_INTERRUPTS_ON_COMMS
	noInterrupts();
#endif

	while (!serialCheckRxComplete())
	{
		;;
	}

#ifdef NO_INTERRUPTS_ON_COMMS
	interrupts();
#endif

	return UDR0;
}
unsigned int ReadInt()
{

#ifdef NO_INTERRUPTS_ON_COMMS
	noInterrupts();
#endif

	unsigned int value;

	while (!serialCheckRxComplete())
	{
		;;
	}

	value = UDR0;

	while (!serialCheckRxComplete())
	{
		;;
	}

	value |= UDR0 << 8;

#ifdef NO_INTERRUPTS_ON_COMMS
	interrupts();
#endif

	return value;

	}
void ReadSegment(volatile byte* target, word length)
{

#ifdef NO_INTERRUPTS_ON_COMMS
	noInterrupts();
#endif

	while (length-- > 0)
	{

		while (!serialCheckRxComplete())
		{
			;;
		}

		*target = (byte)UDR0;
		target++;

	}

#ifdef NO_INTERRUPTS_ON_COMMS
	interrupts();
#endif

}

#pragma endregion
