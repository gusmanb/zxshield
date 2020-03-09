#pragma once
#include <Arduino.h>
#include "ZXShield.h"
//Canales de IO DESDE el Spectrum:
/*
	0: Selector de registro. Al escribir se activa un registro, al leer se lee la info del registro seleccionado
	2: Longitud de registro. Si el registro es de longitud fija leer nos indica el tamaño, si es variable el tamaño de datos
	   disponibles; escribir en él no hace nada si el registro es fijo y cambia el tamaño de datios si es variable
	4: Registro. Lee o escribe dentro del registro activo
	6: Canal libre, se transfiere el control a la app 
*/

#define BYTE_R 0
#define INT_R 1
#define STREAM_R 2

#define REGISTER_CONFIG(type, read, write) (((type) & 3) | ((read) << 2) | ((write) << 3))
#define CONFIG_TYPE(config) ((config) & 3)
#define CONFIG_READ(config) ((config) & 4)
#define CONFIG_WRITE(config) ((config) & 8)
#define CONFIG_DISABLE_READ(config) ((config) & ~4)
#define CONFIG_DISABLE_WRITE(config) ((config) & ~8)
#define CONFIG_ENABLE_READ(config) ((config) | 4)
#define CONFIG_ENABLE_WRITE(config) ((config) | 8)

#define MAX_REGISTER_COUNT 32

typedef struct _REGISTER
{
	volatile byte Config;
	void* Data; //Apunta a los datos del registro, una de las tres structs
	volatile void* UserData; //Puntero donde guardar cualquier información asociada al registro que se desee
	volatile int LockCount; //Indica la cantidad de veces que el registro se ha bloqueado, para uso interno

} REGISTER, *PREGISTER;

typedef void (*REGISTER_CALLBACK)(PREGISTER Register);
typedef byte (*IOREAD_CALLBACK)();
typedef void (*IOWRITE_CALLBACK)(byte Value);

typedef struct _BYTE_REGISTER
{
	volatile byte Value; //Valor almacenado
	REGISTER_CALLBACK ActivateRegister;
	REGISTER_CALLBACK BeginRegisterRead; //Callback ejecutado antes de empezar a leer el registro
	REGISTER_CALLBACK BeginRegisterWrite; //Callback ejecutado antes de empezar a escribir el registro
	REGISTER_CALLBACK EndRegisterRead; //Callback ejecutado al acabar de leer el registro
	REGISTER_CALLBACK EndRegisterWrite; //Callback ejecutado al acabar de escribir el registro

} BYTE_REGISTER, *PBYTE_REGISTER;

typedef struct _INT_REGISTER
{
	volatile unsigned char Buffer[4]; //Valor almacenado
	volatile byte Pointer; //Posición dentro del buffer
	REGISTER_CALLBACK ActivateRegister;
	REGISTER_CALLBACK BeginRegisterRead; //Callback ejecutado antes de empezar a leer el registro
	REGISTER_CALLBACK BeginRegisterWrite; //Callback ejecutado antes de empezar a escribir el registro
	REGISTER_CALLBACK EndRegisterRead; //Callback ejecutado al acabar de leer el registro
	REGISTER_CALLBACK EndRegisterWrite; //Callback ejecutado al acabar de escribir el registro

} INT_REGISTER, *PINT_REGISTER;

typedef struct _STREAM_REGISTER
{
	volatile unsigned char Buffer[64]; //Buffer de datos
	volatile short BufferLength; //Tamaño del buffer, cuando se lee indica cuantos datos hay, cuando se escribe los que va a recibir
	volatile byte Pointer; //Puntero que indica la posición dentro del buffer, cuando un registro se activa este se reinicia a cero. Si llega a 64 se detiene, si se escribe no hace nada y si se lee devuelve 0xFF
	REGISTER_CALLBACK ActivateRegister; 
	REGISTER_CALLBACK BeginSizeRead; //Callback ejecutado antes de leer el tamaño del registro
	REGISTER_CALLBACK BeginSizeWrite; //Callback ejecutado antes de escribir el tamaño del registro
	REGISTER_CALLBACK EndSizeRead; //Callback ejecutado despues de leer el tamaño registro
	REGISTER_CALLBACK EndSizeWrite; //Callback ejecutado despues de escribir el tamaño registro
	REGISTER_CALLBACK BeginRegisterRead; //Callback ejecutado antes de empezar a leer el registro
	REGISTER_CALLBACK BeginRegisterWrite; //Callback ejecutado antes de empezar a escribir el registro
	REGISTER_CALLBACK EndRegisterRead; //Callback ejecutado al acabar de leer el registro
	REGISTER_CALLBACK EndRegisterWrite; //Callback ejecutado al acabar de escribir el registro
} STREAM_REGISTER, *PSTREAM_REGISTER;

class ZXPeripheral
{

public:
	
	IOREAD_CALLBACK ReadRequest = NULL;
	IOWRITE_CALLBACK WriteRequest = NULL;

	PREGISTER CreateRegister(
		byte RegNumber,
		byte Type,
		bool AllowRead,
		bool AllowWrite,
		REGISTER_CALLBACK ActivateRegister = NULL,
		REGISTER_CALLBACK BeginRegisterRead = NULL,
		REGISTER_CALLBACK BeginRegisterWrite = NULL,
		REGISTER_CALLBACK EndRegisterRead = NULL,
		REGISTER_CALLBACK EndRegisterWrite = NULL,
		REGISTER_CALLBACK BeginSizeRead = NULL,
		REGISTER_CALLBACK BeginSizeWrite = NULL,
		REGISTER_CALLBACK EndSizeRead = NULL,
		REGISTER_CALLBACK EndSizeWrite = NULL);
	PREGISTER GetRegister(byte RegNumber);
	void DeleteRegister(byte RegNumber);
	void DataReady(byte Port, byte Write);
	static void SetByteValue(PREGISTER Register, byte Value);
	static void SetIntValue(PREGISTER Register, long Value);
	static void SetStreamValue(PREGISTER Register, const char * Value, byte Length);
	static void SetStreamValue(PREGISTER Register, const char * Value);
	static byte GetByteValue(PREGISTER Register);
	static long GetIntValue(PREGISTER Register);
	static int GetStreamValue(PREGISTER Register, char * Target);
	static int GetStreamLength(PREGISTER Register);
	static void LockRegister(PREGISTER Register);
	static void UnlockRegister(PREGISTER Register);

	volatile byte CurrentRegisterId() {		return currentRegisterId; }
private:
	volatile PREGISTER Registers[MAX_REGISTER_COUNT];
	volatile PREGISTER CurrentRegister = NULL;
	volatile PBYTE_REGISTER CurrentByte = NULL;
	volatile PINT_REGISTER CurrentInt = NULL;
	volatile PSTREAM_REGISTER CurrentStream = NULL;
	volatile uint8_t currentRegisterId;
	volatile uint8_t* releasePort;
	uint8_t retainBit;
	uint8_t releaseBit;

	void ActivateRegister(byte RegNumber);
	void SendRegisterInfo();
	void GetRegisterLength();
	void SetRegisterLength(byte Data);
	void ReadRegister();
	void WriteRegister(byte Data);
};
