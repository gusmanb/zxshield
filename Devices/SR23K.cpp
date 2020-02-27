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

byte SR23K::readByte(const dword address)
{

    startCommand(SR23K_COMMAND_READ, address);
    byte ret = spi->transfer(SR23K_DUMMY_BYTE);
    endCommand();

    return(ret);
}

dword SR23K::writeByte(const dword address, const byte byte)
{

    startCommand(SR23K_COMMAND_WRITE, address);
    spi->transfer(byte);
    endCommand();
    return(1);
}

dword SR23K::readBlock(const dword address, const dword length, void* buffer)
{

    startCommand(SR23K_COMMAND_READ, address);
    spi->transfer(buffer, length);
    endCommand();

    return(length);
}

dword SR23K::writeBlock(const dword address, const dword length, void* buffer)
{
    byte* buf = reinterpret_cast<byte*>(buffer);
    dword len = length;

    startCommand(SR23K_COMMAND_WRITE, address);
    while (len--) {
        spi->transfer(*buf);
        buf++;
    }
    endCommand();

    return(length);
}

void SR23K::startCommand(const byte command, const dword address)
{
    spi->beginTransaction(settings);
    digitalWrite(mCsPin, LOW);

    spi->transfer(command);
    if (command == SR23K_COMMAND_READ || command == SR23K_COMMAND_WRITE)
        sendAddressBytes(address);
}

void SR23K::sendAddressBytes(const dword address)
{
    spi->transfer((byte)((address >> 8) & 0xFF));
    spi->transfer((byte)(address & 0xFF));
}

void SR23K::endCommand(void)
{
    digitalWrite(mCsPin, HIGH);
    spi->endTransaction();
}