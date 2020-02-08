#pragma once

#include <Arduino.h>
#include "ZXPeripheral.h"
#include "ZXROM.h"

#define DIR_IN 0
#define DIR_OUT 0xFF

#define ADDR_L_PORT PINA
#define ADDR_L_DIR DDRA
#define ADDR_L_PORT_MASK B00000110

#define ADDR_H_OP_PORT PINC
#define ADDR_H_OP_DIR DDRC
#define ADDR_H_OP_DIRMASK B10000000
#define ADDR_H_MASK B00111111
#define OP_MASK B01000000

#define RST_PORT PORTC
#define RST_HI_MASK B10000000
#define RST_LO_MASK B01111111

#define DATA_DIR DDRL
#define DATA_IN PINL
#define DATA_OUT PORTL

#define CTL_DIR DDRG
#define CTL_DIRMASK B00000111
#define CTL_PORT PORTG
#define RELEASE_HI_MASK B00000001
#define RELEASE_LO_MASK B11111110
#define NMI_HI_MASK   B00000010
#define NMI_LO_MASK   B11111101
#define ENROM_HI_MASK B00000100
#define ENROM_LO_MASK B11111011

#define ENPERI_DIR DDRD
#define ENPERI_DIRMASK B10000000
#define ENPERI_PORT PORTD
#define ENPERI_HI_MASK B10000000
#define ENPERI_LO_MASK B01111111

#define INT_PERI_PIN 2
#define INT_ROM_PIN 3
#define RAISE_INT_PIN 5

#define PIN_HI(PORT, HI_MASK) ((PORT) |= (HI_MASK))
#define PIN_LO(PORT, LO_MASK) ((PORT) &= (LO_MASK))

#define RELEASE_CPU_TRAP() (PIN_LO(CTL_PORT, RELEASE_LO_MASK))
#define REARM_CPU_TRAP() (PIN_HI(CTL_PORT, RELEASE_HI_MASK))

#define SIGNAL_NMI() (PIN_LO(CTL_PORT, NMI_LO_MASK))
#define FREE_NMI() (PIN_HI(CTL_PORT, NMI_HI_MASK))

#define SIGNAL_INT() (digitalWrite(RAISE_INT_PIN, 0));
#define FREE_INT() (digitalWrite(RAISE_INT_PIN, 1))

#define SIGNAL_RESET() (PIN_LO(RST_PORT, RST_LO_MASK))
#define FREE_RESET() (PIN_HI(RST_PORT, RST_HI_MASK))

#define ENABLE_PERIPHERAL() PIN_HI(ENPERI_PORT, ENPERI_HI_MASK)
#define DISABLE_PERIPHERAL() PIN_LO(ENPERI_PORT, ENPERI_LO_MASK)

#define ENABLE_ROM() PIN_HI(CTL_PORT, ENROM_HI_MASK)
#define DISABLE_ROM() PIN_LO(CTL_PORT, ENROM_LO_MASK)

class ZXPeripheral;
class ZXROM;

class ZXShield
{
public:
	
	static ZXPeripheral Peripheral;
	static ZXROM ROM;

	static inline void Initialize();

	static inline byte InputByte();
	static inline void OutputPeripheralByte(byte Value);
	static inline void OutputROMByte(byte Value);
	static inline void FastOutputByte(byte Value);
	static inline void ReleaseTrap();
	static inline void ResetCPU();
	static inline void NMI();
	static inline void INT();

	static inline void EnablePeripheral();
	static inline void DisablePeripheral();

	static inline void EnableROM();
	static inline void EnableROMWithNMI();
	static inline void DisableROM();

private:

	static void PeripheralInterrupt();
	static void ROMInterrupt();

};


inline void ZXShield::Initialize()
{
	ADDR_L_DIR = DIR_IN;
	ADDR_H_OP_DIR = ADDR_H_OP_DIRMASK;
	DATA_DIR = DIR_IN;
	CTL_DIR = CTL_DIRMASK;
	ENPERI_DIR = ENPERI_DIRMASK;

	//TODO: Create masks and so on
	pinMode(RAISE_INT_PIN, OUTPUT);

	FREE_INT();
	FREE_NMI();
	FREE_RESET();

	REARM_CPU_TRAP();
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();
	interrupts();

}

inline byte ZXShield::InputByte()
{
	byte value = DATA_IN;
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();

	return value;
}

inline void ZXShield::OutputPeripheralByte(byte Value)
{
	DATA_DIR = DIR_OUT;
	DATA_OUT = Value;
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();
	asm("nop");
	asm("nop");
	asm("nop");
	DATA_DIR = DIR_IN;
}

inline void ZXShield::OutputROMByte(byte Value)
{
	DATA_DIR = DIR_OUT;
	DATA_OUT = Value;
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	//asm("nop");
	//asm("nop");
	DATA_DIR = DIR_IN;
}

inline void ZXShield::FastOutputByte(byte Value)
{
	DATA_DIR = DIR_OUT;
	DATA_OUT = Value;
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();
	DATA_DIR = DIR_IN;
}

inline void ZXShield::ReleaseTrap()
{
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();
}

inline void ZXShield::ResetCPU()
{
	SIGNAL_RESET();
	delayMicroseconds(100);
	REARM_CPU_TRAP();
	RELEASE_CPU_TRAP();
	REARM_CPU_TRAP();
	FREE_RESET();
}

inline void ZXShield::NMI()
{
	SIGNAL_NMI();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	FREE_NMI();
}

inline void ZXShield::INT()
{
	SIGNAL_INT();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	FREE_INT();
}

inline void ZXShield::EnablePeripheral()
{
	attachInterrupt(digitalPinToInterrupt(INT_PERI_PIN), PeripheralInterrupt, FALLING);
	ENABLE_PERIPHERAL();
}

inline void ZXShield::DisablePeripheral()
{
	detachInterrupt(digitalPinToInterrupt(INT_PERI_PIN));
	DISABLE_PERIPHERAL();
}

inline void ZXShield::EnableROM()
{
	attachInterrupt(digitalPinToInterrupt(INT_ROM_PIN), ROMInterrupt, FALLING);
	ENABLE_ROM();
}

inline void ZXShield::EnableROMWithNMI()
{
	attachInterrupt(digitalPinToInterrupt(INT_ROM_PIN), ROMInterrupt, FALLING);
	byte status = CTL_PORT;
	status &= NMI_LO_MASK;
	status |= ENROM_HI_MASK;
	//Signal NMI and enable ROM in same operation
	CTL_PORT = status;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	FREE_NMI();
}

inline void ZXShield::DisableROM()
{
	detachInterrupt(digitalPinToInterrupt(INT_ROM_PIN));
	DISABLE_ROM();
}
