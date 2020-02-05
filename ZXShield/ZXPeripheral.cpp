#include "ZXPeripheral.h"

PREGISTER ZXPeripheral::CreateRegister(
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
	REGISTER_CALLBACK EndSizeWrite = NULL)
{

	if (RegNumber > 31 || Type > STREAM_R)
		return NULL;

	PREGISTER newRegister = NULL;

	if (Type == BYTE_R)
	{
		newRegister = (PREGISTER)malloc(sizeof(REGISTER));
		memset(newRegister, 0, sizeof(REGISTER));
		newRegister->Config = REGISTER_CONFIG(BYTE_R, AllowRead ? 1 : 0, AllowWrite ? 1 : 0);
		PBYTE_REGISTER bReg = (PBYTE_REGISTER)malloc(sizeof(BYTE_REGISTER));
		memset(bReg, 0, sizeof(BYTE_REGISTER));
		bReg->ActivateRegister = ActivateRegister;
		bReg->BeginRegisterRead = BeginRegisterRead;
		bReg->BeginRegisterWrite = BeginRegisterWrite;
		bReg->EndRegisterRead = EndRegisterRead;
		bReg->EndRegisterWrite = EndRegisterWrite;
		bReg->Value = 0;
		newRegister->Data = bReg;
	}
	else if (Type == INT_R)
	{
		newRegister = (PREGISTER)malloc(sizeof(REGISTER));
		memset(newRegister, 0, sizeof(REGISTER));
		newRegister->Config = REGISTER_CONFIG(INT_R, AllowRead ? 1 : 0, AllowWrite ? 1 : 0);
		PINT_REGISTER iReg = (PINT_REGISTER)malloc(sizeof(INT_REGISTER));
		memset(iReg, 0, sizeof(INT_REGISTER));
		iReg->ActivateRegister = ActivateRegister;
		iReg->BeginRegisterRead = BeginRegisterRead;
		iReg->BeginRegisterWrite = BeginRegisterWrite;
		iReg->EndRegisterRead = EndRegisterRead;
		iReg->EndRegisterWrite = EndRegisterWrite;
		newRegister->Data = iReg;
	}
	else if (Type == STREAM_R)
	{
		newRegister = (PREGISTER)malloc(sizeof(REGISTER));
		memset(newRegister, 0, sizeof(REGISTER));
		newRegister->Config = REGISTER_CONFIG(STREAM_R, AllowRead ? 1 : 0, AllowWrite ? 1 : 0);
		PSTREAM_REGISTER sReg = (PSTREAM_REGISTER)malloc(sizeof(STREAM_REGISTER));
		memset(sReg, 0, sizeof(STREAM_REGISTER));
		sReg->ActivateRegister = ActivateRegister;
		sReg->BeginRegisterRead = BeginRegisterRead;
		sReg->BeginRegisterWrite = BeginRegisterWrite;
		sReg->EndRegisterRead = EndRegisterRead;
		sReg->EndRegisterWrite = EndRegisterWrite;
		sReg->BeginSizeRead = BeginSizeRead;
		sReg->BeginSizeWrite = BeginSizeWrite;
		sReg->EndSizeRead = EndSizeRead;
		sReg->EndSizeWrite = EndSizeWrite;
		newRegister->Data = sReg;
	}

	Registers[RegNumber] = newRegister;
	
	return newRegister;
}

PREGISTER ZXPeripheral::GetRegister(byte RegNumber)
{
	if (RegNumber > 31)
		return NULL;

	return Registers[RegNumber];
}

void ZXPeripheral::DeleteRegister(byte RegNumber)
{
	if (RegNumber > 31)
		return;

	PREGISTER reg = Registers[RegNumber];

	if (reg == NULL)
		return;

	free(reg->Data);
	free(reg);
}

void ZXPeripheral::SetByteValue(PREGISTER Register, byte Value)
{
	((PBYTE_REGISTER)Register->Data)->Value = Value;
}

void ZXPeripheral::SetIntValue(PREGISTER Register, long Value)
{
	PINT_REGISTER reg = (PINT_REGISTER)Register->Data;
	reg->Pointer = 0;
	memcpy((void*)reg->Buffer, &Value, 4);
}

void ZXPeripheral::SetStreamValue(PREGISTER Register, const char * Value, byte Length)
{
	PSTREAM_REGISTER reg = (PSTREAM_REGISTER)Register->Data;
	reg->BufferLength = Length;
	memcpy((void*)reg->Buffer, Value, Length);
}

void ZXPeripheral::SetStreamValue(PREGISTER Register, const char * Value)
{
	int len = strlen(Value);
	PSTREAM_REGISTER reg = (PSTREAM_REGISTER)Register->Data;
	reg->BufferLength = len;
	memcpy((void*)reg->Buffer, Value, len);
}

byte ZXPeripheral::GetByteValue(PREGISTER Register)
{
	return ((PBYTE_REGISTER)Register->Data)->Value;
}

long ZXPeripheral::GetIntValue(PREGISTER Register)
{
	PINT_REGISTER reg = (PINT_REGISTER)Register->Data;
	return (reg->Buffer[0] << 24) | (reg->Buffer[1] << 16) | (reg->Buffer[2] << 8) | reg->Buffer[3];
}

int ZXPeripheral::GetStreamValue(PREGISTER Register, char * Target)
{
	PSTREAM_REGISTER reg = (PSTREAM_REGISTER)Register->Data;
	memcpy(Target, (void*)reg->Buffer, reg->BufferLength);
	return reg->BufferLength;
}

int ZXPeripheral::GetStreamLength(PREGISTER Register)
{
	PSTREAM_REGISTER reg = (PSTREAM_REGISTER)Register->Data;
	return reg->BufferLength;
}

void ZXPeripheral::LockRegister(PREGISTER Register)
{
	if(Register->LockCount < 255)
		Register->LockCount++;
}

void ZXPeripheral::UnlockRegister(PREGISTER Register)
{
	if(Register->LockCount > 0)
		Register->LockCount--;
}

//Private
void ZXPeripheral::DataReady(byte Port, byte Write)
{

	switch (Port)
	{
	case 0: //Selector

		if (Write) //Escritura
			ActivateRegister(ZXShield::InputByte());
		else
			SendRegisterInfo();

		break;
	case 2: //Longitud
		if (Write) //Escritura
			SetRegisterLength(ZXShield::InputByte());
		else
			GetRegisterLength();
		break;
	case 4:
		if (Write) //Escritura
			WriteRegister(ZXShield::InputByte());
		else
			ReadRegister();
		break;
	case 6:
		if (Write)
		{
			if (WriteRequest != NULL)
				WriteRequest(ZXShield::InputByte());
			else
				ZXShield::ReleaseTrap();
		}
		else
		{
			if (ReadRequest != NULL)
				ZXShield::OutputPeripheralByte(ReadRequest());
			else
				ZXShield::ReleaseTrap();
		}
	default:
		ZXShield::ReleaseTrap();
		break;
	}
}

void ZXPeripheral::ActivateRegister(byte RegNumber)
{
	if (RegNumber > MAX_REGISTER_COUNT)
		return;

	CurrentRegister = Registers[RegNumber];

	switch (CONFIG_TYPE(CurrentRegister->Config))
	{
	case BYTE_R:

		CurrentByte = (PBYTE_REGISTER)CurrentRegister->Data;
		CurrentInt = NULL;
		CurrentStream = NULL;
		
		if (CurrentByte->ActivateRegister != NULL)
			CurrentByte->ActivateRegister(CurrentRegister);

		break;
	case INT_R:

		CurrentByte = NULL;
		CurrentInt = (PINT_REGISTER)CurrentRegister->Data;
		CurrentStream = NULL;
		CurrentInt->Pointer = 0;

		if (CurrentInt->ActivateRegister != NULL)
			CurrentInt->ActivateRegister(CurrentRegister);

		break;
	case STREAM_R:

		CurrentByte = NULL;
		CurrentInt = NULL; 
		CurrentStream = (PSTREAM_REGISTER)CurrentRegister->Data;
		CurrentStream->Pointer = 0;

		if (CurrentStream->ActivateRegister != NULL)
			CurrentStream->ActivateRegister(CurrentRegister);

		break;
	}
}

void ZXPeripheral::SendRegisterInfo()
{
	if (CurrentRegister == NULL)
		ZXShield::OutputPeripheralByte(0xFF);
	else
		ZXShield::OutputPeripheralByte(CurrentRegister->Config);
}

void ZXPeripheral::SetRegisterLength(byte Data)
{

	if (Data > 64 || CurrentRegister == NULL || !CONFIG_WRITE(CurrentRegister->Config) || CONFIG_TYPE(CurrentRegister->Config) != STREAM_R || CurrentRegister->LockCount > 0)
		return;

	if (CurrentStream->BeginSizeWrite != NULL)
		CurrentStream->BeginSizeWrite(CurrentRegister);

	CurrentStream->BufferLength = Data;
	CurrentStream->Pointer = 0;

	if (CurrentStream->EndSizeWrite != NULL)
		CurrentStream->EndSizeWrite(CurrentRegister);
}

void ZXPeripheral::GetRegisterLength()
{

	if (CurrentRegister == NULL)
	{
		ZXShield::OutputPeripheralByte(0xFF);
	}
	else if (CurrentRegister->LockCount > 0)
	{
		ZXShield::OutputPeripheralByte(0xFA);
	}
	else
	{
		switch (CONFIG_TYPE(CurrentRegister->Config))
		{
		case BYTE_R:

			ZXShield::OutputPeripheralByte((byte)1);

			break;

		case INT_R:

			ZXShield::OutputPeripheralByte((byte)4);

			break;

		case STREAM_R:

			if (CurrentStream->BeginSizeRead != NULL)
				CurrentStream->BeginSizeRead(CurrentRegister);

			ZXShield::OutputPeripheralByte(CurrentStream->BufferLength);

			if (CurrentStream->EndSizeRead != NULL)
				CurrentStream->EndSizeRead(CurrentRegister);

			break;

		default:

			ZXShield::OutputPeripheralByte(0xFF);
			break;
		}
	}
}

void ZXPeripheral::WriteRegister(byte Value)
{
	if (CurrentRegister == NULL || !CONFIG_WRITE(CurrentRegister->Config) || CurrentRegister->LockCount > 0)
		return;

	switch (CONFIG_TYPE(CurrentRegister->Config))
	{
	case BYTE_R:

		if (CurrentByte->BeginRegisterWrite != NULL)
			CurrentByte->BeginRegisterWrite(CurrentRegister);

		CurrentByte->Value = Value;

		if (CurrentByte->EndRegisterWrite != NULL)
			CurrentByte->EndRegisterWrite(CurrentRegister);

		break;

	case INT_R:

		if (CurrentInt->Pointer >= 4)
			return;
		else
		{
			if (CurrentInt->Pointer == 0 && CurrentInt->BeginRegisterWrite != NULL)
				CurrentInt->BeginRegisterWrite(CurrentRegister);

			CurrentInt->Buffer[CurrentInt->Pointer++] = Value;

			if (CurrentInt->Pointer == 4 && CurrentInt->EndRegisterWrite != NULL)
				CurrentInt->EndRegisterWrite(CurrentRegister);
			
		}
		break;

	case STREAM_R:

		if (CurrentStream->Pointer >= CurrentStream->BufferLength)
			return;
		else
		{
			if (CurrentStream->Pointer == 0 && CurrentStream->BeginRegisterWrite != NULL)
				CurrentStream->BeginRegisterWrite(CurrentRegister);

			CurrentStream->Buffer[CurrentStream->Pointer++] = Value;

			if (CurrentStream->Pointer == CurrentStream->BufferLength && CurrentStream->EndRegisterWrite != NULL)
				CurrentStream->EndRegisterWrite(CurrentRegister);
		}

		break;
	}
}

void ZXPeripheral::ReadRegister()
{
	if (CurrentRegister == NULL || !CONFIG_READ(CurrentRegister->Config) || CurrentRegister->LockCount > 0)
		ZXShield::OutputPeripheralByte(0xFF);
	else
	{
		switch (CONFIG_TYPE(CurrentRegister->Config))
		{
		case BYTE_R:

			if (CurrentByte->BeginRegisterRead != NULL)
				CurrentByte->BeginRegisterRead(CurrentRegister);

			ZXShield::OutputPeripheralByte(CurrentByte->Value);

			if (CurrentByte->EndRegisterRead != NULL)
				CurrentByte->EndRegisterRead(CurrentRegister);

			break;
		case INT_R:

			if (CurrentInt->Pointer >= 4)
				ZXShield::OutputPeripheralByte(0xFF);
			else
			{
				if (CurrentInt->Pointer == 0 && CurrentInt->BeginRegisterRead != NULL)
					CurrentInt->BeginRegisterRead(CurrentRegister);

				ZXShield::OutputPeripheralByte(CurrentInt->Buffer[CurrentInt->Pointer++]);

				if (CurrentInt->Pointer == 4 && CurrentInt->EndRegisterRead != NULL)
					CurrentInt->EndRegisterRead(CurrentRegister);
			}
			break;

		case STREAM_R:

			if (CurrentStream->Pointer == CurrentStream->BufferLength)
				ZXShield::OutputPeripheralByte(0xFF);
			else
			{
				if (CurrentStream->Pointer == 0 && CurrentStream->BeginRegisterRead != NULL)
					CurrentStream->BeginRegisterRead(CurrentRegister);

				ZXShield::OutputPeripheralByte(CurrentStream->Buffer[CurrentStream->Pointer++]);

				if (CurrentStream->Pointer == CurrentStream->BufferLength && CurrentStream->EndRegisterRead != NULL)
					CurrentStream->EndRegisterRead(CurrentRegister);
			}

			break;

		default:

			ZXShield::OutputPeripheralByte(0xFF);

			break;
		}
	}
}
