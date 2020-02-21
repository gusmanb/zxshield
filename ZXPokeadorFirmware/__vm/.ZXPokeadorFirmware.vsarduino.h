/* 
	Editor: https://www.visualmicro.com/
			This file is for intellisense purpose only.
			Visual micro (and the arduino ide) ignore this code during compilation. This code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			The contents of the _vm sub folder can be deleted prior to publishing a project
			All non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			Note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Nucleo-144, Platform=stm32, Package=STM32
*/

#if defined(_VMICRO_INTELLISENSE)

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define STM32H7xx
#define ARDUINO 108010
#define ARDUINO_NUCLEO_H743ZI2
#define ARDUINO_ARCH_STM32
#define STM32H743xx
#define HAL_UART_MODULE_ENABLED
#define __cplusplus 201103L

#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__

#define __GNUC__ 6
#define  __GNUC_MINOR__ 3
#define  __GNUC_PATCHLEVEL__ 0
#define _Static_assert(x)

#undef __cplusplus
#define __cplusplus 201103L
typedef bool _Bool;
#define __ARMCC_VERSION 400678
#define __attribute__(noinline)



#include "arduino.h"
#include <PinNamesVar.h> 
#include <variant.h> 
#include <variant.cpp> 
#undef cli
#define cli()
#include "ZXPokeadorFirmware.ino"
#endif
#endif
