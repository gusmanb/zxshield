#include "ZXShield.h"

ZXPeripheral ZXShield::Peripheral;
ZXROM ZXShield::ROM;

void ZXShield::PeripheralInterrupt()
{
	byte port = ADDR_L_PORT & ADDR_L_PORT_MASK;
	byte op = ADDR_H_OP_PORT & OP_MASK;

	Peripheral.DataReady(port, op);
}

void ZXShield::ROMInterrupt()
{
	unsigned int addr = ADDR_L_PORT;
	byte op = ADDR_H_OP_PORT;
	addr |= (op & ADDR_H_MASK) << 8;
	op = op & OP_MASK;
	ROM.DataReady(addr, op);
}