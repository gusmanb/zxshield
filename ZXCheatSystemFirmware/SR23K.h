#pragma once
#include <arduino.h>
#include <SPI.h>
typedef unsigned long dword;

// Commands supported by all chips
#define SR23K_COMMAND_READ			0x03
#define SR23K_COMMAND_WRITE			0x02
#define SR23K_COMMAND_RDMR			0x05
#define SR23K_COMMAND_WRMR			0x01

// Additional commands supported by some chips
#define SR23K_COMMAND_EDIO			0x3B
#define SR23K_COMMAND_EQIO			0x38
#define SR23K_COMMAND_RSTIO			0xFF


#define SR23K_MODE_Pos			6
#define SR23K_MODE_Msk			(0x3ul << SR23K_MODE_Pos)
#define SR23K_MODE(value)			(SR23K_MODE_Msk & ((value) << SR23K_MODE_Pos))
#define SR23K_MODE_BYTE_Val		0x0ul
#define SR23K_MODE_SEQUENTIAL_Val		0x1ul
#define SR23K_MODE_PAGE_Val		0x2ul
#define SR23K_MODE_BYTE			(SR23K_MODE_BYTE_Val << SR23K_MODE_Pos)
#define SR23K_MODE_SEQUENTIAL		(SR23K_MODE_SEQUENTIAL_Val << SR23K_MODE_Pos)
#define SR23K_MODE_PAGE			(SR23K_MODE_PAGE_Val << SR23K_MODE_Pos)

#define SR23K_DUMMY_BYTE			0xFF

class SR23K
{
public:
    SR23K(SPIClass* SPI, byte csPin, dword spiSpeed);

    uint8_t readByte(const uint32_t address);

    size_t writeByte(const uint32_t address, const uint8_t byte);

    size_t readBlock(const uint32_t address, const size_t length, void* buffer);
    size_t writeBlock(const uint32_t address, const size_t length, void* buffer);



    void startCommand(const uint8_t command, const uint32_t address);
    void sendAddressBytes(const uint32_t address);
    void endCommand(void);

private:
    byte mCsPin;
    dword mSpiSpeed;
    SPISettings settings;
    SPIClass* spi;
};

