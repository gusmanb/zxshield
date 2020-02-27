#ifndef S25FL208K_h
#define S25FL208K_h

#include <arduino.h>
#include <SPI.h>

typedef uint32_t dword;

const byte WRITE_ENABLE = 0x06;
const byte WRITE_DISABLE = 0x04;
const byte READ_STATUS_REG = 0x05;
const byte WRITE_STATUS_REG = 0x01;
const byte READ_DATA = 0x03;
const byte FAST_READ = 0x0B;
const byte FAST_READ_DUAL_OUTPUT = 0x3B;
const byte PAGE_PROGRAM = 0x02;
const byte BLOCK_ERASE = 0xD8; // 64kb
const byte SECTOR_ERASE = 0x20; // 4kb
const byte CHIP_ERASE = 0xC7; // or 0x60;
const byte POWER_DOWN = 0xB9;
const byte RELEASE_POWER_DOWN = 0xAB;
const byte DEV_ID = 0x90;
const byte JEDEC_ID = 0x9F;
const byte DUMMY_VALUE = 0x00;
const byte WIP_MASK = 0x01;

class S25FL208K {
public:
    S25FL208K(SPIClass* SPI, byte csPin, dword spiSpeed, word maxWIPRetries);
    byte readStatus();
    bool readData(dword address, dword numBytes, byte* result);
    void writeEnable(void);
    void writeDisable(void);
    bool getWIP(void);
    bool chipErase(void);
    bool sectorErase(dword address);
    bool blockErase(dword address);
    bool storeData(dword address, word numBytes, byte* data);

    //void beginTransaction();
    //void endTransaction();

private:
    void sendCommand(byte command, byte bytesToRead = 0, byte* outputData = NULL);
    bool waitWIP();
    byte mCsPin;
    dword mSpiSpeed;
    word mMaxWIPRetries;
    SPISettings settings;
    SPIClass* spi;
};

#endif