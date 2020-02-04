#pragma once
class RingBuffer
{
public:
	RingBuffer(int BufferSize);
	~RingBuffer();
	void Add(unsigned char Value);
	bool Next(unsigned char* Value);
private:
	unsigned char* buffer;
	int readPos;
	int writePos;
	int size;
};

