#include "ESP01.h"

ESP01::ESP01(unsigned long Bauds)
{
	bauds = Bauds;
}

bool ESP01::Open()
{
	ESP_SERIAL.begin(bauds);
	return true;
}

bool ESP01::Close()
{
	ESP_SERIAL.end();
	return true;
}

bool ESP01::ConnectWiFi(const char* SSID, const char* Password)
{
	SendCommand("+++");

	delay(100);

	ClearBuffer();

	if (!SendCommand(F("AT+RST")))
	{
		//Serial.println("Error rst");
		return false;
	}
		

	delay(2000);

	ClearBuffer();

	if (!SendCommand(F("ATE0")))
	{
		//Serial.println("Error ATE0");
		return false;
	}

	delay(2000);

	ClearBuffer();

	if (!SendCommand(F("AT+CWMODE=1")))
	{
		//Serial.println("Error CWMODE");
		return false;
	}

	if (!GetCommandResponse())
	{
		//Serial.println("Error CWMODERESPONSE");
		return false;
	}

	if (!SendPartialCommand(F("AT+CWJAP_CUR=\"")))
		return false;

	if (!SendPartialCommand(SSID))
		return false;

	if (!SendPartialCommand(F("\",\"")))
		return false;

	if (!SendPartialCommand(Password))
		return false;

	if (!SendCommand(F("\"")))
		return false;

	if (!GetCommandResponse())
		return false;




	if (!SendCommand("AT+CIFSR"))
		return false;

	if (!GetCommandResponse())
		return false;

	if (!ParseIP())
		return false;




	if (!SendCommand(F("AT+CIPMUX=0")))
		return false;

	if (!GetCommandResponse())
		return false;

	if (!SendCommand(F("AT+CIPRECVMODE=1")))
		return false;

	if (!GetCommandResponse())
		return false;

	return true;
}

bool ESP01::ParseIP()
{
	int line = FindLine("+CIFSR:STAIP,", 13);

	if (line < 0)
		return false;

	memset(IPAddress, 0, IP_BUFFER_LEN);

	line += 14;
	int pos = 0;

	while (CommandData[line] != '"')
		IPAddress[pos++] = CommandData[line++];
	
	return true;
}

bool ESP01::CloseWiFi()
{
	if (!SendCommand(F("AT+RST")))
		return false;

	delay(2000);

	ClearBuffer();

	return true;
}

bool ESP01::ConnectToServer(const char * Address, const char * Port)
{
	if (!SendPartialCommand(F("AT+CIPSTART=\"TCP\",\"")))
		return false;

	if (!SendPartialCommand(Address))
		return false;

	if (!SendPartialCommand(F("\",")))
		return false;

	if (!SendCommand(Port))
		return false;

	if (!GetCommandResponse())
		return false;
	
	return true;
}

bool ESP01::DisconnectFromServer()
{
	if (!SendCommand(F("AT+CIPCLOSE")))
		return false;

	if (!GetCommandResponse())
		return false;

	return true;
}

bool ESP01::IsTCPConnected()
{
	if (!SendCommand(F("AT+CIPSTATUS")))
		return false;

	if (!GetCommandResponse())
		return false;

	int pos = FindLine("STATUS:3", 8);

	//Serial.println("LINEA...");
	//Serial.println(pos);

	if (pos == -1)
		return false;

	return true;
}

bool ESP01::SendCommand(const char * Command)
{
	ClearBuffer();

	//Serial.print(Command);
	//Serial.print(F("\r\n"));
	ESP_SERIAL.print(Command);
	ESP_SERIAL.print(F("\r\n"));

	return true;
}

bool ESP01::SendCommand(const __FlashStringHelper * Command)
{
	ClearBuffer();

	//Serial.print(Command);
	//Serial.print(F("\r\n"));
	ESP_SERIAL.print(Command);
	ESP_SERIAL.print(F("\r\n"));

	return true;
}

bool ESP01::SendPartialCommand(const char * Command)
{
	//Serial.print(Command);
	ESP_SERIAL.print(Command);

	return true;
}

bool ESP01::SendPartialCommand(const __FlashStringHelper * Command)
{
	//Serial.print(Command);
	ESP_SERIAL.print(Command);

	return true;
}

void ESP01::ClearBuffer()
{
	while (ESP_SERIAL.available())
		ESP_SERIAL.read();
}

bool ESP01::SendData(const char* Data, int Length)
{
	char sizeBuffer[10];

	if (itoa(Length, sizeBuffer, 10) == NULL)
		return false;

	if (!SendPartialCommand(F("AT+CIPSEND=")))
		return false;

	if (!SendCommand(sizeBuffer))
		return false;

	if (!GetCommandResponse())
		return false;

	ESP_SERIAL.write(Data, Length);
	ClearBuffer();

	return GetCommandResponse();

}

int ESP01::GetDataLength()
{
	//Serial.print("PIDE TAM");

	memset(CommandData, 0, 512);

	if (!SendCommand(F("AT+CIPRECVLEN?")))
	{
		//Serial.println("Error envio");
		return -1;
	}

	if (!GetCommandResponse())
	{
		//Serial.println("Error respuesta");
		return -2;
	}

	if (CommandDataCount != 1)
	{
		//Serial.println("Error cantidad comandos");
		//Serial.println(CommandDataCount);

		//for (int buc = 0; buc < CommandDataCount; buc++)
			//Serial.println(CommandLineLength[buc]);

		//Serial.println(CommandData);
		return -3;
	}

	int posStart = IndexOf(CommandData, CommandLineLength[0], ':');
	int posEnd = IndexOf(CommandData, CommandLineLength[0] + posEnd, ',');

	if (posStart < 0 || posEnd <= posStart)
	{
		//Serial.println("Error de busqueda de tam");
		//Serial.println("posStart");
		//Serial.println("posEnd");
		return -4;
	}

	int value;

	int len = posEnd - (posStart + 1);
	
	//Serial.println("Len");
	//Serial.println(len);


	char buf[6];
	memset(buf, 0, 6);

	memcpy(buf, &CommandData[posStart + 1], len);

	//Serial.println("Str len");
	//Serial.println(buf);

	value = atoi(buf);

	return value;

}

int ESP01::GetData(char* Buffer, int Length)
{

	char lenBuffer[6];

	if (itoa(Length, lenBuffer, 10) == NULL)
	{
		//Serial.println("Error len A");
		return -1;
	}

	if (!SendPartialCommand(F("AT+CIPRECVDATA=")))
	{
		//Serial.println("Error len B");
		return -2;
	}

	if (!SendCommand(lenBuffer))
	{
		//Serial.println("Error len C");
		return -1;
	}

	ReadLine(CommandData, CMD_BUF_LEN);

	//Serial.println("Linea descartada");
	//Serial.println(CommandData);
	//Serial.println("----");
	for (int buc = 0; buc < 13; buc++)
	{
		while (!ESP_SERIAL.available())
			continue;

		ESP_SERIAL.read();

		//Serial.print((char)ESP_SERIAL.read());
	}

	int len = -4;

	for (int buc = 0; buc < 6; buc++)
	{
		while (!ESP_SERIAL.available())
			continue;

		lenBuffer[buc] = (char)ESP_SERIAL.read();
		char c = (char)lenBuffer[buc];
		//Serial.println(c);
		if (c == ':')
		{
			//Serial.println("Len leida: ");

			lenBuffer[buc] = 0;

			//Serial.println(lenBuffer);

			len = atoi(lenBuffer);

			break;
		}
		else if (c == '\n')
		{
			//Serial.println("Barra n!");
			break;
		}
	}

	if (len < 0)
	{
		ClearBuffer();
		return len;
	}

	ESP_SERIAL.readBytes(Buffer, len);

	GetCommandResponse();

	return len;
}

//Modificar para usar el modo pasivo haciendo polling
int ESP01::GetDataResponse(char* Buffer, int Length, int RequiredLength)
{
	int iLen = 0;
	char res = ' ';
	int bufPos = 0;

	if (RequiredLength == 0)
	{
		bool found = false;

		while (ESP_SERIAL.available())
		{
			res = (char)ESP_SERIAL.read();

			if (res == '+')
			{
				found = true;
				break;
			}
		}

		if (!found)
			return 0;

		ESP_SERIAL.read();
		ESP_SERIAL.read();
		ESP_SERIAL.read();
		ESP_SERIAL.read();

		char sizeBuffer[10];
		found = false;
		
		while (!found)
		{

			res = (char)ESP_SERIAL.read();

			if (res == ':')
			{
				sizeBuffer[bufPos] = 0;
				found = true;
			}
			else
			{
				if (bufPos >= 9)
					return false;

				sizeBuffer[bufPos++] = res;
			}
		}

		iLen = atoi(sizeBuffer);

		if (iLen > RequiredLength)
			return iLen * -1;
	}
	else
		iLen = RequiredLength;

	bufPos = 0;

	while (bufPos < iLen)
		Buffer[bufPos++] = (char)ESP_SERIAL.read();

	return iLen;
}

bool ESP01::GetCommandResponse()
{
	char response[128];
	int dataPos = 0;
	CommandDataCount = 0;
	memset(&CommandLineLength, 0, sizeof(int) * CMD_LINE_LEN);

	//Serial.println("--CMDRESPONSE--");

	while (true)
	{
		int lineLen = ReadLine(response, 128);

		//Serial.println(lineLen);
		//Serial.println(response);

		if (lineLen < 0)
			return false;

		if (!strcmp_P(response, PSTR("OK")) || !strcmp_P(response, PSTR("SEND OK")))
			return true;
		else if (!strcmp_P(response, PSTR("ERROR")) || !strcmp_P(response, PSTR("FAIL")))
			return false;
		else if (StartsWith(response, lineLen, "+IPD", 4))
			continue;
		else
		{
			int free = CMD_BUF_LEN - dataPos;

			if (free >= lineLen && lineLen != 0)
			{
				memcpy(&CommandData[dataPos], response, lineLen);
				CommandLineLength[CommandDataCount++] = lineLen;
				dataPos += lineLen;
			}
		}
	}
}

int ESP01::ReadLine(char* Buffer, int Length)
{
	memset(Buffer, 0, Length);
	int pos = 0;

	while (true)
	{
		int loopCount = 0;

		while (!ESP_SERIAL.available())//No data, wait...
		{
			loopCount++;
			delay(1);

			if (loopCount > 30000) //Timeout!
				return -1;

		}

		while (ESP_SERIAL.available() && pos < Length - 1)//While something to read...
		{
			char val = (char)ESP_SERIAL.read(); //Store in buffer
			Buffer[pos++] = val;

			if (val == '\n') //Line terminator?
			{
				pos -= 2; //Remove the new line
				Buffer[pos] = '\0'; //Place a string terminator
				return pos; //Return line length
			}
		}

		if (pos >= Length - 1) //Overflow!!
			return -2;
	}

	return -3;
}

int ESP01::FindLine(const char* Buffer, int Length)
{
	int bufPos = 0;

	for (int buc = 0; buc < CMD_LINE_LEN; buc++)
	{
		int len = CommandLineLength[buc];

		if (len == 0)
			return -1;
		else
		{
			if (StartsWith(&CommandData[bufPos], len, Buffer, Length))
				return bufPos;
			else
				bufPos += len;
		}
	}

	return -1;
}

bool ESP01::StartsWith(const char* Where, int WhereLength, const char* What, int WhatLength)
{
	if (WhereLength < WhatLength)
		return false;

	bool matches = true;
	for (int pos = 0; pos < WhatLength; pos++)
	{
		if (Where[pos] != What[pos])
		{
			matches = false;
			break;
		}
	}

	return matches;
}

int ESP01::IndexOf(const char* Buffer, int Length, const char Search, int StartIndex = 0)
{
	for (int buc = StartIndex; buc < Length; buc++)
	{
		if (Buffer[buc] == Search)
			return buc;
	}

	return - 1;
}