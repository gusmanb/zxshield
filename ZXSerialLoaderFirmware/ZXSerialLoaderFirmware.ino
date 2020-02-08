/*
 Name:		ZXSerialLoaderFirmware.ino
 Created:	07/02/2020 1:23:08
 Author:	geniw
*/

#include "arduino.h"
#include "ZXShield.h"
#include <avr/io.h> 

#define NO_INTERRUPTS_ON_COMMS

#define STATUS_ADDRESS			0
#define START_ADDRESS_ADDRESS	1
#define SEGMENT_DEST_ADDRESS	3
#define SEGMENT_SIZE_ADDRESS	5

#define EXIT_ADDRESS			0x98
#define SEGMENT_ADDRESS			0x99

volatile byte* volatile status;			//Pointer to status in the virtual RAM
volatile word* volatile startAddress;	//Pointer to start address in virtual RAM
volatile word* volatile segmentDest;	//Pointer to segment destination in virtual RAM
volatile word* volatile segmentSize;	//Pointer to segment size in virtual RAM
volatile byte* volatile virtualSegment;	//Pointer to the segment in virtual RAM

volatile byte virtualRAM[4096];			//Virtual RAM buffer

volatile bool vramDisabled = false;

// the setup function runs once when you press reset or power the board
void setup() 
{
	//Clear virtual RAM
	memset((void*)virtualRAM, 0, 4096);

	#pragma region Virtual RAM initializer
	virtualRAM[0] = 0x00;
	virtualRAM[1] = 0x00;
	virtualRAM[2] = 0x00;
	virtualRAM[3] = 0x00;
	virtualRAM[4] = 0x00;
	virtualRAM[5] = 0x00;
	virtualRAM[6] = 0x00;
	virtualRAM[7] = 0x00;
	virtualRAM[8] = 0x00;
	virtualRAM[9] = 0x00;
	virtualRAM[10] = 0x00;
	virtualRAM[11] = 0x00;
	virtualRAM[12] = 0x00;
	virtualRAM[13] = 0x00;
	virtualRAM[14] = 0x00;
	virtualRAM[15] = 0x00;
	virtualRAM[16] = 0x00;
	virtualRAM[17] = 0x00;
	virtualRAM[18] = 0x00;
	virtualRAM[19] = 0x00;
	virtualRAM[20] = 0x00;
	virtualRAM[21] = 0x00;
	virtualRAM[22] = 0x00;
	virtualRAM[23] = 0x00;
	virtualRAM[24] = 0x00;
	virtualRAM[25] = 0x00;
	virtualRAM[26] = 0x00;
	virtualRAM[27] = 0x00;
	virtualRAM[28] = 0x00;
	virtualRAM[29] = 0x00;
	virtualRAM[30] = 0x00;
	virtualRAM[31] = 0x00;
	virtualRAM[32] = 0x00;
	virtualRAM[33] = 0x00;
	virtualRAM[34] = 0x00;
	virtualRAM[35] = 0x00;
	virtualRAM[36] = 0x00;
	virtualRAM[37] = 0x00;
	virtualRAM[38] = 0x00;
	virtualRAM[39] = 0x00;
	virtualRAM[40] = 0x00;
	virtualRAM[41] = 0x00;
	virtualRAM[42] = 0x00;
	virtualRAM[43] = 0x00;
	virtualRAM[44] = 0x00;
	virtualRAM[45] = 0x00;
	virtualRAM[46] = 0x00;
	virtualRAM[47] = 0x00;
	virtualRAM[48] = 0x00;
	virtualRAM[49] = 0x00;
	virtualRAM[50] = 0x00;
	virtualRAM[51] = 0x00;
	virtualRAM[52] = 0x00;
	virtualRAM[53] = 0x00;
	virtualRAM[54] = 0x00;
	virtualRAM[55] = 0x00;
	virtualRAM[56] = 0x00;
	virtualRAM[57] = 0x00;
	virtualRAM[58] = 0x00;
	virtualRAM[59] = 0x00;
	virtualRAM[60] = 0x00;
	virtualRAM[61] = 0x00;
	virtualRAM[62] = 0x00;
	virtualRAM[63] = 0x00;
	virtualRAM[64] = 0x00;
	virtualRAM[65] = 0x00;
	virtualRAM[66] = 0x00;
	virtualRAM[67] = 0x00;
	virtualRAM[68] = 0x00;
	virtualRAM[69] = 0x00;
	virtualRAM[70] = 0x00;
	virtualRAM[71] = 0x00;
	virtualRAM[72] = 0x00;
	virtualRAM[73] = 0x00;
	virtualRAM[74] = 0x00;
	virtualRAM[75] = 0x00;
	virtualRAM[76] = 0x00;
	virtualRAM[77] = 0x00;
	virtualRAM[78] = 0x00;
	virtualRAM[79] = 0x00;
	virtualRAM[80] = 0x00;
	virtualRAM[81] = 0x00;
	virtualRAM[82] = 0x00;
	virtualRAM[83] = 0x00;
	virtualRAM[84] = 0x00;
	virtualRAM[85] = 0x00;
	virtualRAM[86] = 0x00;
	virtualRAM[87] = 0x00;
	virtualRAM[88] = 0x00;
	virtualRAM[89] = 0x00;
	virtualRAM[90] = 0x00;
	virtualRAM[91] = 0x00;
	virtualRAM[92] = 0x00;
	virtualRAM[93] = 0x00;
	virtualRAM[94] = 0x00;
	virtualRAM[95] = 0x00;
	virtualRAM[96] = 0x00;
	virtualRAM[97] = 0x00;
	virtualRAM[98] = 0x00;
	virtualRAM[99] = 0x00;
	virtualRAM[100] = 0x00;
	virtualRAM[101] = 0x00;

	virtualRAM[102] = 0xF3; //DI

	virtualRAM[103] = 0xE1; //POP HL

	virtualRAM[104] = 0x2A; //LD HL, (startAddress)
	virtualRAM[105] = 0x01;
	virtualRAM[106] = 0x00;

	virtualRAM[107] = 0xE5; //PUSH HL

	virtualRAM[108] = 0x21; //LD HL, segmentSource
	virtualRAM[109] = 0x99;
	virtualRAM[110] = 0x00;

	virtualRAM[111] = 0xED; //LD DE, (segmentDest)
	virtualRAM[112] = 0x5B;
	virtualRAM[113] = 0x03;
	virtualRAM[114] = 0x00;

	virtualRAM[115] = 0xED; //LD BC, (segmentSize)
	virtualRAM[116] = 0x4B;
	virtualRAM[117] = 0x05;
	virtualRAM[118] = 0x00;

	virtualRAM[119] = 0xED; //LDIR
	virtualRAM[120] = 0xB0;

	virtualRAM[121] = 0x3E; //LD A, 1
	virtualRAM[122] = 0x01;

	virtualRAM[123] = 0x32; //LD(status), A
	virtualRAM[124] = 0x00;
	virtualRAM[125] = 0x00;

	virtualRAM[126] = 0x3A;
	virtualRAM[127] = 0x00;
	virtualRAM[128] = 0x00;
	virtualRAM[129] = 0xEE;
	virtualRAM[130] = 0xAA;
	virtualRAM[131] = 0xCA;
	virtualRAM[132] = 0x96;
	virtualRAM[133] = 0x00;
	virtualRAM[134] = 0x3A;
	virtualRAM[135] = 0x00;
	virtualRAM[136] = 0x00;
	virtualRAM[137] = 0xEE;
	virtualRAM[138] = 0x55;
	virtualRAM[139] = 0xC2;
	virtualRAM[140] = 0x7E;
	virtualRAM[141] = 0x00;
	virtualRAM[142] = 0x3E;
	virtualRAM[143] = 0x00;
	virtualRAM[144] = 0x32;
	virtualRAM[145] = 0x00;
	virtualRAM[146] = 0x00;
	virtualRAM[147] = 0xC3;
	virtualRAM[148] = 0x6C;
	virtualRAM[149] = 0x00;
	virtualRAM[150] = 0xFB;
	virtualRAM[151] = 0xED;
	virtualRAM[152] = 0x45;
	#pragma endregion

	//Create pointers to special addresses on the vRAM buffer
	status = &virtualRAM[STATUS_ADDRESS];
	startAddress = (volatile word* volatile)&virtualRAM[START_ADDRESS_ADDRESS];
	segmentDest = (volatile word* volatile)&virtualRAM[SEGMENT_DEST_ADDRESS];
	segmentSize = (volatile word* volatile)&virtualRAM[SEGMENT_SIZE_ADDRESS];
	virtualSegment = &virtualRAM[SEGMENT_ADDRESS];

	//Initialize the shield and vRAM handler
	ZXShield::Initialize();
	ZXShield::ROM.SetROMHandler(RAMHandler);

	//Reset CPU (for sanity)
	ZXShield::ResetCPU();

	//Initialize serial port
	Serial.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
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

		if (Address == EXIT_ADDRESS) //Did the speccy read the last instruction?
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
