#include "RingBuffer.h"
#include <stdlib.h>
#include <string.h>

RingBuffer::RingBuffer(int BufferSize)
{
	size = BufferSize;
	buffer = (unsigned char*)malloc(BufferSize);
	memset(buffer, 0, BufferSize);
	readPos = 0;
	writePos = 0;
}

RingBuffer::~RingBuffer()
{
	free(buffer);
}

void RingBuffer::Add(unsigned char Value)
{
	buffer[writePos++] = Value;

	if (writePos >= size)
		writePos = 0;
	
	if (readPos == writePos)
		readPos++;

	if (readPos >= size)
		readPos = 0;
	
}

bool RingBuffer::Next(unsigned char * Value)
{
	if (readPos == writePos)
		return false;

	*Value = buffer[readPos++];

	if (readPos >= size)
		readPos = 0;

	return true;
}
