// ESP01.h

#ifndef _ESP01_h
#define _ESP01_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"

#else
#include "WProgram.h"
#endif

#define CMD_BUF_LEN 512
#define CMD_LINE_LEN 10
#define IP_BUFFER_LEN 17

#ifndef ESP_SERIAL
#define ESP_SERIAL Serial1
#endif

class ESP01
{
public:
	char IPAddress[IP_BUFFER_LEN];
	ESP01(unsigned long Bauds);
	bool Open();
	bool Close();
	bool ConnectWiFi(const char* SSID, const char* Password);
	bool CloseWiFi();
	bool ConnectToServer(const char* Address, const char * Port);
	bool DisconnectFromServer();
	bool IsTCPConnected();
	bool SendCommand(const char* Command);
	bool SendCommand(const __FlashStringHelper* Command);
	bool SendPartialCommand(const char* Command);
	bool SendPartialCommand(const __FlashStringHelper* Command);
	void ClearBuffer();
	bool SendData(const char* Data, int Length);
	int GetDataLength();
	int GetData(char* Buffer, int Length);
	int GetDataResponse(char* Buffer, int Length, int RequiredLength);
	
private:
	unsigned long bauds;
	int CommandLineLength[CMD_LINE_LEN];
	char CommandData[CMD_BUF_LEN];
	int CommandDataCount;
	
	bool GetCommandResponse();
	bool ParseIP();
	int ReadLine(char* Buffer, int Length);
	int FindLine(const char* Buffer, int Length);
	bool StartsWith(const char* Where, int WhereLength, const char* What, int WhatLength);
	int IndexOf(const char* Buffer, int Length, const char Search, int StartIndex = 0);

};

#endif


