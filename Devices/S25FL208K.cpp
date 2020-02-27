#include "S25FL208K.h"
#include <SPI.h>

S25FL208K::S25FL208K(SPIClass* SPI, byte csPin, dword spiSpeed, word maxWIPRetries)
{
    spi = SPI;
    mSpiSpeed = spiSpeed;
    mCsPin = csPin;
    mMaxWIPRetries = maxWIPRetries;
    spi->begin();
    pinMode(mCsPin, OUTPUT);
    digitalWrite(mCsPin, 1); // active low, start disabled
    settings = SPISettings(spiSpeed, MSBFIRST, SPI_MODE0);
}

void S25FL208K::sendCommand(byte command, byte bytesToRead, byte* outputData) 
{
    spi->beginTransaction(this->settings);
    digitalWrite(mCsPin, 0);
    spi->transfer(command);
    if (bytesToRead > 0) {
        for (int i = 0; i < bytesToRead; i++) {
            outputData[i] = spi->transfer(DUMMY_VALUE);
        }
    }
    digitalWrite(mCsPin, 1);
    spi->endTransaction();
}

bool S25FL208K::waitWIP()
{
    word retries = mMaxWIPRetries;

    while (retries-- > 0)
    {
        if (this->getWIP())
            delay(1);
        else
            return true;
    }

    return false;
}

byte S25FL208K::readStatus() 
{
    byte result;
    sendCommand(READ_STATUS_REG, 1, &result);
    return result;
}

void S25FL208K::writeEnable(void) {
    sendCommand(WRITE_ENABLE);
}

void S25FL208K::writeDisable(void) {
    sendCommand(WRITE_DISABLE);
}

bool S25FL208K::getWIP(void) {
    return (this->readStatus() & WIP_MASK);
}

bool S25FL208K::readData(unsigned long address, dword numBytes, byte* result) 
{
    if (!this->waitWIP())
        return false;

    spi->beginTransaction(this->settings);
    digitalWrite(mCsPin, 0);
    // transfer the address as 3 bytes
    spi->transfer(READ_DATA);
    spi->transfer(byte(0xFF & (address >> 16)));
    spi->transfer(byte(0xFF & (address >> 8)));
    spi->transfer(byte(0xFF & address));

    // read the data into result
    for (int i = 0; i < numBytes; i++) {
        result[i] = spi->transfer(DUMMY_VALUE);
    }
    // end transfer
    digitalWrite(mCsPin, 1);
    spi->endTransaction();
    return true;
}

bool S25FL208K::storeData(dword address, word numBytes, byte* data) 
{
    if (!this->waitWIP())
        return false;

    // check page erased
    bool allOnes = true;
    byte* tempResult = new byte[numBytes];
    this->readData(address, numBytes, tempResult);
    for (int i = 0; i < numBytes; i++) {
        if (tempResult[i] != 0xFF) {
            allOnes = false;
        }
    }
    delete[] tempResult;

    if (!allOnes) {
        return false;
    }

    // set WRITE_ENABLE
    this->writeEnable();

    spi->beginTransaction(this->settings);

    digitalWrite(mCsPin, 0);
    // send address
    spi->transfer(PAGE_PROGRAM);
    spi->transfer(byte(0xFF & (address >> 16)));
    spi->transfer(byte(0xFF & (address >> 8)));
    spi->transfer(byte(0xFF & address));
    //send data
    for (int i = 0; i < numBytes; i++) {
        spi->transfer(data[i]);
    }

    digitalWrite(mCsPin, 1);

    spi->endTransaction();

    return true;
}

bool S25FL208K::chipErase(void) 
{
    if (!this->waitWIP())
        return false;

    // set WRITE_ENABLE
    this->writeEnable();

    this->sendCommand(CHIP_ERASE);
    return true;
}

bool S25FL208K::sectorErase(dword address) 
{
    if (!this->waitWIP())
        return false;

    // set WRITE_ENABLE
    this->writeEnable();
    
    spi->beginTransaction(this->settings);
    digitalWrite(mCsPin, 0);
    // transfer the address as 3 bytes
    spi->transfer(SECTOR_ERASE);
    spi->transfer(byte(0xFF & (address >> 16)));
    spi->transfer(byte(0xFF & (address >> 8)));
    spi->transfer(byte(0xFF & address));
    digitalWrite(mCsPin, 1);
    spi->endTransaction();

    return true;
}

bool S25FL208K::blockErase(dword address) 
{
    if (!this->waitWIP())
        return false;

    // set WRITE_ENABLE
    this->writeEnable();

    spi->beginTransaction(this->settings);
    digitalWrite(mCsPin, 0);
    // transfer the address as 3 bytes
    spi->transfer(BLOCK_ERASE);
    spi->transfer(byte(0xFF & (address >> 16)));
    spi->transfer(byte(0xFF & (address >> 8)));
    spi->transfer(byte(0xFF & address));
    digitalWrite(mCsPin, 1);
    spi->endTransaction();

    return true;
}