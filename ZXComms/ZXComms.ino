/*
    Name:       ZXComms.ino
    Created:	24/01/2020 23:29:34
    Author:     DESKTOP-C983CQ7\geniw
*/

#define ESP_SERIAL Serial1

#include "RingBuffer.h"
#include <Arduino.h>
#include "ZXShield.h"
#include "ZXPeripheral.h"
#include "ESP01.h"

#define REG_SSID 0
#define REG_PASS 1
#define REG_WIFI 2
#define REG_ADDR 3
#define REG_PORT 4
#define REG_TCPS 5
#define REG_DINP 6
#define REG_DOUT 7
#define REG_INFO 8

#define OP_STORE_SSID 0
#define OP_STORE_PASS 1
#define OP_CHANGE_WIFI 2
#define OP_STORE_ADDR 3
#define OP_STORE_PORT 4
#define OP_STATUS_TCP 5
#define OP_CHANGE_TCP 6
#define OP_READ_DATA 7
#define OP_WRITE_DATA 8

RingBuffer* opBuffer;
ESP01* wifi;
char ssid[33];
byte ssidLen;
char password[33];
byte passLen;
char address[65];
byte addrLen;
int port;

char dataIn[64];
byte dataInLen;

char dataOut[64];
byte dataOutLen;

// The setup() function runs once each time the micro-controller starts
/*Registros:
0 = SSID, stream
1 = PASSWORD, stream
2 = Conexion al WiFi, byte, lectura da el estado, escritura permite conectar/desconectar
3 = dirección, stream
4 = puerto, entero
5 = conexión TCP, byte, lectura da el estado, escritura permite conectar/desconectar. 0 = desconectado, 1 = conectando, 2 = conectado, 3 = ocupado
6 = buffer entrada, stream, solo escritura, lo que se escriba en este stream se envía por TCP
7 = buffer de salida, stream, solo lectura, cuando se lee el tamaño del stream se verifica la cantidad de datos en el modem, si hay se copian hasta 64 en él*/


void setup()
{
	wifi = new ESP01((unsigned long)115200);
	wifi->Open();

	ZXShield::Initialize();
	opBuffer = new RingBuffer(16);

	ZXShield::Peripheral.CreateRegister(REG_SSID, STREAM_R, false, true, NULL, NULL, NULL, NULL, storeSSID); //asincrono, bloquea a si mismo y a 2
	ZXShield::Peripheral.CreateRegister(REG_PASS, STREAM_R, false, true, NULL, NULL, NULL, NULL, storePassword); //asincrono, bloquea a si mismo y a 2
	/*
	Valores:
	0 = en reposo
	1 = solicitando conexion
	2 = conectado
	3 = error al conectar
	4 = desconectado
	5 = error al desconectar
	*/
	ZXShield::Peripheral.CreateRegister(REG_WIFI, BYTE_R, true, true, NULL, NULL, NULL, NULL, setWiFiStatus); //asincrono, bloquea a si mismo y a 0 y 1
	ZXShield::Peripheral.CreateRegister(REG_ADDR, STREAM_R, true, true, NULL, NULL, NULL, NULL, storeAddress);//asincrono, bloquea a si mismo y a 5
	ZXShield::Peripheral.CreateRegister(REG_PORT, INT_R, true, true, NULL, NULL, NULL, NULL, storePort); //asincrono, bloquea a si mismo y a 5
	/*
	Valores:
	0 = desconectado
	1 = solicitando conexion
	2 = conectado
	*/
	ZXShield::Peripheral.CreateRegister(REG_TCPS, BYTE_R, true, true, getTCPStatus, NULL, NULL, NULL, setTCPStatus); //sincrono/asincrono, bloquea a si mismo y a 3 y 4
	ZXShield::Peripheral.CreateRegister(REG_DOUT, STREAM_R, false, true, NULL, NULL, NULL, NULL, sendData); //asincrono, bloquea a si mismo
	ZXShield::Peripheral.CreateRegister(REG_DINP, STREAM_R, true, false, receiveDataSize, NULL, NULL, NULL, NULL, NULL); //sincrono
	ZXShield::Peripheral.CreateRegister(REG_INFO, STREAM_R, true, false); //Asincrono, da info del estado

	ZXShield::EnablePeripheral();
}

void storeSSID(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	PREGISTER stat = ZXShield::Peripheral.GetRegister(REG_WIFI);
	ZXPeripheral::LockRegister(stat);
	opBuffer->Add(OP_STORE_SSID);
}

void storePassword(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	PREGISTER stat = ZXShield::Peripheral.GetRegister(REG_WIFI);
	ZXPeripheral::LockRegister(stat);
	opBuffer->Add(OP_STORE_PASS);
}

void setWiFiStatus(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	PREGISTER reg = ZXShield::Peripheral.GetRegister(REG_SSID);
	ZXPeripheral::LockRegister(reg);
	reg = ZXShield::Peripheral.GetRegister(REG_PASS);
	ZXPeripheral::LockRegister(reg);
	opBuffer->Add(OP_CHANGE_WIFI);
}

void storeAddress(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	PREGISTER stat = ZXShield::Peripheral.GetRegister(REG_TCPS);
	ZXPeripheral::LockRegister(stat);
	opBuffer->Add(OP_STORE_ADDR);
}

void storePort(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	PREGISTER stat = ZXShield::Peripheral.GetRegister(REG_TCPS);
	ZXPeripheral::LockRegister(stat);
	opBuffer->Add(OP_STORE_PORT);
}

void getTCPStatus(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	opBuffer->Add(OP_STATUS_TCP);
}

void setTCPStatus(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	PREGISTER reg = ZXShield::Peripheral.GetRegister(REG_ADDR);
	ZXPeripheral::LockRegister(reg);
	reg = ZXShield::Peripheral.GetRegister(REG_PORT);
	ZXPeripheral::LockRegister(reg);
	opBuffer->Add(OP_CHANGE_TCP);
}

void sendData(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	opBuffer->Add(OP_WRITE_DATA);
}

void receiveDataSize(PREGISTER Target)
{
	ZXPeripheral::LockRegister(Target);
	opBuffer->Add(OP_READ_DATA);		
}

void loop()
{
	unsigned char cmd;
	PREGISTER reg;
	PREGISTER regInfo = ZXShield::Peripheral.GetRegister(REG_INFO);

	while (opBuffer->Next(&cmd))
	{
		if (cmd == OP_STORE_SSID)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Storing SSID");
			reg = ZXShield::Peripheral.GetRegister(REG_SSID);
			ssidLen = ZXPeripheral::GetStreamValue(reg, ssid);
			ssid[ssidLen] = 0;
			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_WIFI);
			ZXPeripheral::UnlockRegister(reg);
			ZXPeripheral::SetStreamValue(regInfo, "SSID stored");
		}
		else if (cmd == OP_STORE_PASS)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Storing pass");
			reg = ZXShield::Peripheral.GetRegister(REG_PASS);
			passLen = ZXPeripheral::GetStreamValue(reg, password);
			password[passLen] = 0;
			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_WIFI);
			ZXPeripheral::UnlockRegister(reg);
			ZXPeripheral::SetStreamValue(regInfo, "Pass stored");
			break;
		}
		else if (cmd == OP_CHANGE_WIFI)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Processing WiFi");
			reg = ZXShield::Peripheral.GetRegister(REG_WIFI);
			byte status = ZXPeripheral::GetByteValue(reg);

			if (status == 1)//Conectar
			{
				ZXPeripheral::SetStreamValue(regInfo, "Connecting to WiFi");

				if (wifi->ConnectWiFi(ssid, password))
				{
					ZXPeripheral::SetByteValue(reg, 2);
					ZXPeripheral::SetStreamValue(regInfo, "Connected to WiFi");
				}
				else
				{
					ZXPeripheral::SetStreamValue(regInfo, "Error connecting to WiFi");
					ZXPeripheral::SetByteValue(reg, 3);
				}
			}
			else
			{
				ZXPeripheral::SetStreamValue(regInfo, "Closing WiFi");

				if (wifi->CloseWiFi())
				{
					ZXPeripheral::SetStreamValue(regInfo, "WiFi closed");
					ZXPeripheral::SetByteValue(reg, 4);
				}
				else
				{
					ZXPeripheral::SetStreamValue(regInfo, "Error closing WiFi");
					ZXPeripheral::SetByteValue(reg, 5);
				}
			}

			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_SSID);
			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_PASS);
			ZXPeripheral::UnlockRegister(reg);
		}
		else if (cmd == OP_STORE_ADDR)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Storing address");

			reg = ZXShield::Peripheral.GetRegister(REG_ADDR);
			addrLen = ZXPeripheral::GetStreamValue(reg, address);
			address[addrLen] = 0;
			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_TCPS);
			ZXPeripheral::UnlockRegister(reg);

			ZXPeripheral::SetStreamValue(regInfo, "Address stored");
		}
		else if (cmd == OP_STORE_PORT)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Storing port");

			reg = ZXShield::Peripheral.GetRegister(REG_PORT);
			port = ZXPeripheral::GetIntValue(reg);
			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_TCPS);
			ZXPeripheral::UnlockRegister(reg);

			ZXPeripheral::SetStreamValue(regInfo, "Port stored");
		}
		else if (cmd == OP_STATUS_TCP)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Reading TCP status");
			reg = ZXShield::Peripheral.GetRegister(REG_TCPS);
			bool val = wifi->IsTCPConnected();

			if (val)
				ZXPeripheral::SetByteValue(reg, 2);
			else
				ZXPeripheral::SetByteValue(reg, 3);

			ZXPeripheral::UnlockRegister(reg);
			ZXPeripheral::SetStreamValue(regInfo, "Status read");
		}
		else if (cmd == OP_CHANGE_TCP)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Processing TCP");

			reg = ZXShield::Peripheral.GetRegister(REG_TCPS);
			byte tstatus = ZXPeripheral::GetByteValue(reg);

			if (tstatus == 1)//Conectar
			{
				ZXPeripheral::SetStreamValue(regInfo, "Connecting to server");

				char prtASCII[10];
				itoa(port, prtASCII, 10);

				if (wifi->ConnectToServer(address, prtASCII))
				{
					ZXPeripheral::SetStreamValue(regInfo, "Connected to server");
					ZXPeripheral::SetByteValue(reg, 2);
				}
				else
				{
					ZXPeripheral::SetStreamValue(regInfo, "Error connecting to server");
					ZXPeripheral::SetByteValue(reg, 3);
				}
			}
			else
			{
				ZXPeripheral::SetStreamValue(regInfo, "Disconnecting from server");
				wifi->DisconnectFromServer();
				ZXPeripheral::SetStreamValue(regInfo, "Server disconnected");
				ZXPeripheral::SetByteValue(reg, 0);
			}

			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_ADDR);
			ZXPeripheral::UnlockRegister(reg);
			reg = ZXShield::Peripheral.GetRegister(REG_PORT);
			ZXPeripheral::UnlockRegister(reg);
		}
		else if (cmd == OP_READ_DATA)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Processing read data");

			reg = ZXShield::Peripheral.GetRegister(REG_DINP);

			int len = wifi->GetDataLength();

			if (len < 0)
				len = 0;
			else if (len > 64)
				len = 64;

			if (len > 0)
			{
				int rcvLen = wifi->GetData((char*)((PSTREAM_REGISTER)reg->Data)->Buffer, len);

				if (rcvLen < 0)
					((PSTREAM_REGISTER)reg->Data)->BufferLength = 0;
				else
					((PSTREAM_REGISTER)reg->Data)->BufferLength = rcvLen;

			}
			else
				((PSTREAM_REGISTER)reg->Data)->BufferLength = 0;

			ZXPeripheral::UnlockRegister(reg);

			ZXPeripheral::SetStreamValue(regInfo, "Read done");
		}
		else if (cmd == OP_WRITE_DATA)
		{
			ZXPeripheral::SetStreamValue(regInfo, "Sending data");

			reg = ZXShield::Peripheral.GetRegister(REG_DOUT);
			PSTREAM_REGISTER preg = (PSTREAM_REGISTER)reg->Data;
			wifi->SendData((char*)preg->Buffer, preg->BufferLength);
			ZXPeripheral::UnlockRegister(reg);

			ZXPeripheral::SetStreamValue(regInfo, "Data sent");
		}
	}
}