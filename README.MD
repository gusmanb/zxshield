All the content of this reposirtory is published under  CC BY-NC-SA 4.0 license.

This is a work in progress on it's early stages, expect errors and changes.

RECOMMENDED ENVIRONMENT

The code is developed in Visual Studio 2019 community edition. 

- To develop the Arduino firmware I have used Visual Micro: https://www.visualmicro.com/
- To develop the Spectrum programs I have used SpectNetIDE with Boriel Basic:
*	https://github.com/Dotneteer/spectnetide
*	https://github.com/boriel/zxbasic

From my experience this setup is gorgeous, only one IDE for all the system.

The software:

	ZXShield: framework to develop the ZXShield firmware.
	Devices: Shared library to handle various devices like the ESP01 module or the MCP42XXX family of digital potentiometers.
	ZXComms: Peripheral example, it creates a WiFi card for the ZX Spectrum. Needs a ZXShield and a ESP01 module.
	ZXBasicROM: Pasive ROM example. It includes the 48k rom already converted for the system.
	ZXRegisterManager: ZX Spectrum program to make IO requests easily. Very useful to develop and test new devices.
	ZXTelnet: ZX Spectrum telnet client. Used with the ZXComms device.