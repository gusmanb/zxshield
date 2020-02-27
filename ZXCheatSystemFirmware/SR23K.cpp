#include "SR23K.h"

SR23K::SR23K(SPIClass* SPI, byte csPin, dword spiSpeed)
{
    spi = SPI;
    mSpiSpeed = spiSpeed;
    mCsPin = csPin;
    spi->begin();
    pinMode(mCsPin, OUTPUT);
    digitalWrite(mCsPin, 1); // active low, start disabled
    settings = SPISettings(spiSpeed, MSBFIRST, SPI_MODE0);
    startCommand(SR23K_COMMAND_WRMR, 0);
    spi->transfer(SR23K_MODE_SEQUENTIAL);
    endCommand();

}

uint8_t SR23K::readByte(const uint32_t address)
{

    startCommand(SR23K_COMMAND_READ, address);
    uint8_t ret = spi->transfer(SR23K_DUMMY_BYTE);
    endCommand();

    return(ret);
}

size_t SR23K::writeByte(const uint32_t address, const uint8_t byte)
{

    startCommand(SR23K_COMMAND_WRITE, address);
    spi->transfer(byte);
    endCommand();
    return(1);
}

size_t SR23K::readBlock(const uint32_t address, const size_t length, void* buffer)
{

    startCommand(SR23K_COMMAND_READ, address);
    spi->transfer(buffer, length);
    endCommand();

    return(length);
}

size_t SR23K::writeBlock(const uint32_t address, const size_t length, void* buffer)
{
    uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
    size_t len = length;

    startCommand(SR23K_COMMAND_WRITE, address);
    while (len--) {
        spi->transfer(*buf);
        buf++;
    }
    endCommand();

    return(length);
}

void SR23K::startCommand(const uint8_t command, const uint32_t address)
{
    spi->beginTransaction(settings);
    digitalWrite(mCsPin, LOW);

    spi->transfer(command);
    if (command == SR23K_COMMAND_READ || command == SR23K_COMMAND_WRITE)
        sendAddressBytes(address);
}

void SR23K::sendAddressBytes(const uint32_t address)
{
    spi->transfer((uint8_t)((address >> 8) & 0xFF));
    spi->transfer((uint8_t)(address & 0xFF));
}

void SR23K::endCommand(void)
{
    digitalWrite(mCsPin, HIGH);
    spi->endTransaction();
}