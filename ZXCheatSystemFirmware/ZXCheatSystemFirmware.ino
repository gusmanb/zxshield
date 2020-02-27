/*
 Name:		ZXCheatSystemFirmware.ino
 Created:	22/02/2020 21:29:37
 Author:	geniw
*/

#include <Adafruit_ST7735.h>

#include "Arduino.h"
#include "SPI.h"
#include "SR23K.h"
#include "S25FL208K.h"
#include "sembei_small.h"
#include "MenuSystem.h"
#include "ZXShield.h"

#include "PokearProg.h"
#include "LoadProg.h"
#include "DumpProg.h"
#include "SDBrowser.h"
#include "Z80Loader.h"

#define SRAM_CS     8
#define FLASH_CS    9
#define SCREEN_CS   10
#define SCREEN_RS   11
#define SCREEN_DC   12

#define BTN_SELECT  A0
#define BTN_DOWN    A1
#define BTN_UP      A2

#define SD_CS       A3

#define ACTION_SELECTED (!digitalRead(BTN_SELECT))
#define ACTION_UP       (!digitalRead(BTN_UP))
#define ACTION_DOWN     (!digitalRead(BTN_DOWN))

#define PROG_POKE 1
#define PROG_SAVE 2
#define PROG_LOAD 3
#define PROG_Z80  4

#define INTRO_PAGE 0
#define MAIN_PAGE 1
#define POKEADOR_PAGE 2
#define LOAD_SNAPSHOT_PAGE 3
#define SAVE_SNAPSHOT_PAGE 4
#define ERASE_SNAPSHOT_PAGE 5
#define BROWSER_PAGE 6

#define BROWSER_PAGE_SIZE 18

int appPage = 0;

volatile byte* volatile header;			//pointer to SNA header in the virtual RAM
volatile byte* volatile status;			//Pointer to status in the virtual RAM
volatile word* volatile startAddress;	//Pointer to start address in virtual RAM
volatile word* volatile segmentDest;	//Pointer to segment destination in virtual RAM
volatile word* volatile segmentSize;	//Pointer to segment size in virtual RAM
volatile byte* volatile virtualSegment;	//Pointer to the segment in virtual RAM
volatile byte* volatile sramPage;	    //Pointer to the external page number in virtual RAM

volatile byte virtualRAM[4096];			//Virtual RAM buffer

volatile bool vramDisabled = false;

volatile word exitAddress;


SR23K* sram;
S25FL208K* flash;
Adafruit_ST7735* screen;
SDBrowser* browser;

char currentSnapshot;


Menu systemMenu =
{
    .menuTitle = " - CheatSystem 1.0 - ",
    .menuEntries = {
                        "1 - Pokeador         ",
                        "2 - Load snapshot    ",
                        "3 - Save snapshot    ",
                        "4 - Delete snapshot  ",
                        "5 - SD browser       "
                    },
    .menuEntriesCount = 5,
    .currentEntry = 0
};

byte snapshotStatus[BROWSER_PAGE_SIZE];


char browserRecordIndex = 0;
byte browserCurrentPageSize = 0;
bool browserDots = false;


// the setup function runs once when you press reset or power the board
void setup() {

    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, 1);

    Serial.begin(115200);

    Serial.println("INIT");

    ZXShield::Initialize();
    ZXShield::ROM.SetROMHandler(RAMHandler);
    ZXShield::ResetCPU();

    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_UP, INPUT_PULLUP);

    InitScreen();

    flash = new S25FL208K(&SPI, FLASH_CS, 8000000, 5000);
    sram = new SR23K(&SPI, SRAM_CS, 8000000);
    browser = new SDBrowser(SD_CS, 8000000, BROWSER_PAGE_SIZE);

    for (int buc = 0; buc < 256; buc++)
    {
        Serial.println("WRITE");
        Serial.println(buc);
        sram->writeByte(buc, buc);
        Serial.println("READ");
        Serial.println(sram->readByte(buc));
    }

    memset(snapshotStatus, 0, 18);

    Serial.println("READY");
}

void InitScreen()
{
    screen = new Adafruit_ST7735(SCREEN_CS, SCREEN_DC, SCREEN_RS);
    screen->initR();
    screen->fillScreen(ST7735_BLACK);
    uint8_t data = 0xC0;
    screen->sendCommand(ST77XX_MADCTL, &data, 1);
    screen->drawRGBBitmap(0, 0, sembei_upper, 128, 80);
    screen->drawRGBBitmap(0, 80, sembei_lower, 128, 80);
}

void showMessage(const char* message, bool waitConfirm = true)
{
    screen->setTextColor(ST7735_WHITE, ST7735_BLUE);

    screen->setCursor(0, 66);
    screen->println("                     ");
    word mY = screen->getCursorY();
    screen->println("                     ");
    screen->setCursor(0, mY);
    screen->println(message);
    screen->println("                     ");

    if (waitConfirm)
    {
        while (!ACTION_SELECTED)
        {
            delay(10);
        }

        while (ACTION_SELECTED)
        {
            delay(10);
        }
    }
}

bool confirmMessage(const char* message)
{
    screen->setTextColor(ST7735_WHITE, ST7735_BLUE);

    screen->setCursor(0, 66);
    screen->println("                     ");
    word mY = screen->getCursorY();
    screen->println("                     ");
    screen->setCursor(0, mY);
    screen->println(message);
    screen->println("                     ");


    mY = screen->getCursorY();

    screen->setTextColor(ST7735_BLUE, ST7735_WHITE);
    screen->println(" -No                 ");
    screen->setTextColor(ST7735_WHITE, ST7735_BLUE);
    screen->println(" -Yes                ");
    screen->println("                     ");

    bool answer = false;
    bool selected = false;

    while (!selected)
    {
        if (ACTION_SELECTED)
        {
            while (ACTION_SELECTED)
            {
                delay(10);
            }

            selected = true;
            break;
        }
        else if (ACTION_UP || ACTION_DOWN)
        {
            while (ACTION_UP || ACTION_DOWN)
            {
                delay(10);
            }

            answer = !answer;

            screen->setCursor(0, mY);

            if (!answer)
            {
                screen->setTextColor(ST7735_BLUE, ST7735_WHITE);
                screen->println(" -No                 ");
                screen->setTextColor(ST7735_WHITE, ST7735_BLUE);
                screen->println(" -Yes                ");
            }
            else
            {
                screen->setTextColor(ST7735_WHITE, ST7735_BLUE);
                screen->println(" -No                 ");
                screen->setTextColor(ST7735_BLUE, ST7735_WHITE);
                screen->println(" -Yes                ");
            }
        }
    }

    return answer;
}

void showMenu()
{
    screen->setTextColor(ST7735_WHITE, ST7735_BLACK);

    screen->fillScreen(ST7735_BLACK);
    screen->setCursor(0, 1);
    screen->println(systemMenu.menuTitle);
    screen->println();
    screen->println();

    for (int buc = 0; buc < systemMenu.menuEntriesCount; buc++)
    {
        if (buc == systemMenu.currentEntry)
            screen->setTextColor(ST7735_BLACK, ST7735_WHITE);
         
        screen->println(systemMenu.menuEntries[buc]);

        if (buc == systemMenu.currentEntry)
            screen->setTextColor(ST7735_WHITE, ST7735_BLACK);
    }
}

void previousOption()
{
    systemMenu.currentEntry--;

    if (systemMenu.currentEntry < 0)
        systemMenu.currentEntry = systemMenu.menuEntriesCount - 1;

    showMenu();
}

void nextOption()
{
    systemMenu.currentEntry++;

    if (systemMenu.currentEntry >= systemMenu.menuEntriesCount)
        systemMenu.currentEntry = 0;

    showMenu();
}

void showSnapshotSelector()
{
    currentSnapshot = 0;

    readSnapshotStatus();

    screen->setCursor(0, 0);
    screen->fillScreen(ST7735_BLACK);
    screen->setCursor(0, 0);
                    
    screen->println(" - Choose snapshot - ");

    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {

            word color = 0;

            byte currentSnapshot = (y * 4) + x;

            if (snapshotStatus[currentSnapshot] == 1)
                color = ST7735_RED;
            else
                color = ST7735_CYAN;

            word xCoord = x * 32;
            word yCoord = y * 32 + 16;
            
            screen->fillRect(xCoord + 4, yCoord + 4, 24, 24, color);
            
        }
    }

    screen->setCursor(0, 160 - 12);
    screen->setTextColor(ST7735_GREEN, ST7735_RED);
    screen->println("        CANCEL       ");

    showSelectedSnapshot();
}

void readSnapshotStatus()
{
    for (int buc = 0; buc < 16; buc++)
    {
        dword currentSector = buc;
        currentSector = currentSector * 64;
        currentSector = currentSector * 1024;

        byte value;

        Serial.println("STATUS SECTOR");
        Serial.println(currentSector);

        bool res = flash->readData(currentSector, 1, &value);

        if (!res || value != 0xFF)
            snapshotStatus[buc] = 1;
        else
            snapshotStatus[buc] = 0;
    }
}

void showSelectedSnapshot()
{
    if (currentSnapshot == 16)
    {
        screen->setCursor(0, 160 - 12);
        screen->setTextColor(ST7735_RED, ST7735_GREEN);
        screen->println("        CANCEL       ");
    }
    else
    {
        int x = currentSnapshot % 4;
        int y = (currentSnapshot - x) / 4;

        int xCoord = x * 32;
        int yCoord = y * 32 + 16;

        screen->drawRect(xCoord, yCoord, 32, 32, ST7735_YELLOW);
    }
}

void eraseSelectedSnapshot()
{
    if (currentSnapshot == 16)
    {
        screen->setCursor(0, 160 - 12);
        screen->setTextColor(ST7735_GREEN, ST7735_RED);
        screen->println("        CANCEL       ");
    }
    else
    {
        
        int x = currentSnapshot % 4;
        int y = (currentSnapshot - x) / 4;

        int xCoord = x * 32;
        int yCoord = y * 32 + 16;

        screen->drawRect(xCoord, yCoord, 32, 32, ST7735_BLACK);
    }
}

void previousSnapshot()
{
    eraseSelectedSnapshot();

    currentSnapshot--;

    if (currentSnapshot < 0)
        currentSnapshot = 16;

    showSelectedSnapshot();
}

void nextSnapshot()
{
    eraseSelectedSnapshot();

    currentSnapshot++;

    if (currentSnapshot > 16)
        currentSnapshot = 0;

    showSelectedSnapshot();
}

void chooseSnapshot()
{
    switch (appPage)
    {
    case LOAD_SNAPSHOT_PAGE:

        if (snapshotStatus[currentSnapshot])
        {
            loadSnapshot();
            appPage = 1;
            showMenu();
        }
        else
        {
            showMessage("Empty snapshot");
            showSnapshotSelector();
        }

        break;
    case SAVE_SNAPSHOT_PAGE:

        if (!snapshotStatus[currentSnapshot])
        {
            saveSnapshot();
            appPage = 1;
            showMenu();
        }
        else
        {
            showMessage("Used snapshot");
            showSnapshotSelector();
        }

        break;
    case ERASE_SNAPSHOT_PAGE:

        if (snapshotStatus[currentSnapshot])
        {
            if (confirmMessage("Erase?"))
                eraseSnapshot();
                
            showSnapshotSelector();

        }
        else
        {
            showMessage("Empty snapshot");
            showSnapshotSelector();
        }

        break;
    }
}

void loadSnapshot()
{
    loadProgram(PROG_LOAD);

    bool finished = false;
    bool first = true;

    dword readAddress = currentSnapshot;
    readAddress = readAddress * 64;
    readAddress = readAddress * 1024;

    dword writeAddress = 16384;

    byte value = 12;

    Serial.println("READ ADDRES");
    Serial.println(readAddress);

    flash->readData(readAddress, 1, &value);
    readAddress++;

    Serial.println("VALUE");
    Serial.println(value);

    if (value != 0xAA)
    {
        showMessage("Bad snapshot!");
        return;
    }

    showMessage("Loading...", false);

    flash->readData(readAddress, 27, (byte*)header);
    readAddress += 255;

    *status = 0x00;

    byte isLast = 0;

    while (!finished)
    {

        if (!first)
        {
            while (*status != 1);
        }

        dword segmentLength = min(256, 65536 - writeAddress);
        *segmentDest = writeAddress;
        *segmentSize = segmentLength;

        noInterrupts();
        flash->readData(readAddress, segmentLength, (byte*)virtualSegment);
        interrupts();

        readAddress += segmentLength;
        writeAddress += segmentLength;

        isLast = writeAddress >= 65536;

        if (first)
        {
            vramDisabled = false;
            first = false;
            *status = 0x00;
            ZXShield::EnableROMWithNMI();
        }
        else
        {
            if (isLast)	//Was this the last segment?
            {
                finished = true;

                *status = 0x55;			//Signal the program that it has to copy a segment
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
                while (*status != 1);	//Wait until the program returns to the wait loop
                *status = 0xAA;			//Signal the program that we have finished
            }
            else
            {
                *status = 0x55;
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
            }
        }
    }

    while (!vramDisabled);

    showMessage("Snapshot loaded");
    
}

void saveSnapshot()
{

    loadProgram(PROG_SAVE);

    bool finished = false;
    bool first = true;
    bool isLast = false;

    dword flashAddress = currentSnapshot;
    flashAddress = flashAddress * 64;
    flashAddress = flashAddress * 1024;

    dword ramAddress = 16384;

    Serial.println("WRITE SECTOR");
    Serial.println(flashAddress);

    dword segmentLength = min(256, 65536 - ramAddress);

    while (!finished)
    {
        if (first)
        {
            *segmentDest = ramAddress;
            *segmentSize = segmentLength;
            vramDisabled = false;
            *status = 0x00;
            ZXShield::EnableROMWithNMI();

            while (*status != 1);

            byte tmpBuffer[256];
            memset(tmpBuffer, 0, 256);
            tmpBuffer[0] = 0xAA;
            memcpy(&tmpBuffer[1], (byte*)header, 27);

            noInterrupts();
            flash->storeData(flashAddress, 256, tmpBuffer);
            interrupts();

            flashAddress += 256;

            first = false;
        }
        else
        {
            while (*status != 1);

            noInterrupts();
            flash->storeData(flashAddress, segmentLength, (byte*)virtualSegment);
            interrupts();

            flashAddress += segmentLength;
            ramAddress += segmentLength;

            segmentLength = min(256, 65536 - ramAddress);

            *segmentDest = ramAddress;
            *segmentSize = segmentLength;

            isLast = ramAddress + segmentLength >= 65536;

            if (isLast)	//Was this the last segment?
            {
                finished = true;

                *status = 0x55;			//Signal the program that it has to copy a segment
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
                while (*status != 1);	//Wait until the program returns to the wait loop
                *status = 0xAA;			//Signal the program that we have finished
            }
            else
            {
                *status = 0x55;
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
            }
        }
    }

    while (!vramDisabled);

    flash->storeData(flashAddress, segmentLength, (byte*)virtualSegment);

    showMessage("Snapshot Saved");
}

void eraseSnapshot()
{
    dword address = currentSnapshot;
    address = address * 64;
    address = address * 1024;

    bool res = flash->blockErase(address);

    if (res)
        showMessage("Erase completed");
    else
        showMessage("Error erasing");
}

void launchPokeador()
{

    loadProgram(PROG_POKE);
    
    showMessage("Pokeando...", false);

    vramDisabled = false;

    ZXShield::EnableROMWithNMI();
    
    while (!vramDisabled);

}

void showBrowser()
{
    screen->fillScreen(ST7735_BLACK);
    showMessage("Opening card...", false);

    if (!browser->open())
    {
        showMessage("Error opening card");
        return;
    }

    browserRecordIndex = 0;

    showCurrentBrowserPage();

    while (true)
    {
        if (ACTION_SELECTED)
        {
            while (ACTION_SELECTED)
            {
                if (ACTION_DOWN)
                {
                    while (ACTION_DOWN || ACTION_SELECTED);
                    return;
                }
            }

            if (browserDots)
                browserRecordIndex--;

            if (browserRecordIndex == -1)
            {
                browser->parentFolder();
                browserRecordIndex = 0;
                showCurrentBrowserPage();
            }
            else
            {
                if (browser->Page()[browserRecordIndex].isDir)
                {
                    browser->openFolder(browserRecordIndex);
                    browserRecordIndex = 0;
                    showCurrentBrowserPage();
                }
                else
                {
                    SDFile selectedFile = browser->openFile(browserRecordIndex);
                    processFile(selectedFile);
                    browserRecordIndex = 0;
                    showCurrentBrowserPage();

                }
            }
        }
        else if (ACTION_UP)
        {
            int delayup = 0;
            int waitDelay = 1000;

            while (ACTION_UP)
            {
                delay(50);
                delayup += 50;

                if (delayup >= waitDelay)
                {
                    doBrowserActionUp();
                    waitDelay = 50;
                    delayup = 0;
                }
            }

            if(waitDelay == 1000)
                doBrowserActionUp();

        }
        else if (ACTION_DOWN)
        {
            int delaydown = 0;
            int waitDelay = 1000;

            while (ACTION_DOWN)
            {
                delay(50);
                delaydown += 50;

                if (delaydown >= waitDelay)
                {
                    doBrowserActionDown();
                    waitDelay = 50;
                    delaydown = 0;
                }
            }

            if (waitDelay == 1000)
                doBrowserActionDown();

        }
    }
}

void doBrowserActionUp()
{
    if (browserRecordIndex == 0)
    {
        if (!browser->IsFirstPage())
        {
            browser->previousPage();

            if (browser->IsFirstPage() && !browser->IsRoot())
                browserRecordIndex = BROWSER_PAGE_SIZE;
            else
                browserRecordIndex = BROWSER_PAGE_SIZE - 1;

            showCurrentBrowserPage();
        }
    }
    else
    {
        deselectBrowserEntry();
        browserRecordIndex--;
        selectedBroswserEntry();
    }
}

void doBrowserActionDown()
{

    if ((browserDots && browserRecordIndex == BROWSER_PAGE_SIZE) ||
        (!browserDots && browserRecordIndex == BROWSER_PAGE_SIZE - 1))
    {
        if (!browser->IsLastPage())
        {
            browserRecordIndex = 0;
            browser->nextPage();
            showCurrentBrowserPage();
        }
    }
    else
    {
        deselectBrowserEntry();
        browserRecordIndex++;
        selectedBroswserEntry();

    }

}

void processFile(SDFile File)
{
    const char* name = File.name();

    if (!strcasecmp(getExt(name), "sna"))
        loadSNA(File);
    else if (!strcasecmp(getExt(name), "z80"))
        loadZ80(File);
    else
    {
        Serial.println(getExt(name));
        Serial.println(name);
        showMessage("Not supported");
        File.close();
        return;
    }

}

void loadZ80(SDFile File)
{
    showMessage("Creating buffer...", false);

    memset((byte*)virtualRAM, 0xFF, 4096);
    SDFile buffer = browser->createFile("/tmpz80");
    buffer.seek(0);
    
    for (int buc = 0; buc < 16; buc++)
        buffer.write((byte*)virtualRAM, 4096);

    if (!prepareZ80Buffer(File, buffer))
    {
        File.close();
        buffer.close();
        browser->deleteFile("/tmpz80");
        showMessage("Invalid format");
        return;
    }

    File.close();
    buffer.seek(0);

    loadProgram(PROG_Z80);

    bool finished = false;
    bool first = true;

    showMessage("Loading...", false);

    dword address = 16384;

    buffer.readBytes((byte*)header, 29);

    *status = 0x00;

    byte isLast = 0;

    buffer.seek(address);

    while (!finished)
    {

        if (!first)
        {
            while (*status != 1);
        }

        dword segmentLength = min(256, 65536 - address);
        *startAddress = *segmentDest = address;
        *segmentSize = segmentLength;

        noInterrupts();
        buffer.readBytes((byte*)virtualSegment, segmentLength);
        interrupts();

        address += segmentLength;

        isLast = address >= 65536;

        if (first)
        {
            vramDisabled = false;
            first = false;
            *status = 0x00;
            ZXShield::EnableROMWithNMI();
        }
        else
        {
            if (isLast)	//Was this the last segment?
            {
                finished = true;

                *status = 0x55;			//Signal the program that it has to copy a segment
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
                while (*status != 1);	//Wait until the program returns to the wait loop
                *status = 0xAA;			//Signal the program that we have finished
            }
            else
            {
                *status = 0x55;
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
            }
        }
    }

    while (!vramDisabled);

    buffer.close();
    browser->deleteFile("/tmpz80");
    showMessage("Z80 loaded");

}

bool prepareZ80Buffer(SDFile file, SDFile buffer)
{
    showMessage("Preparing buffer...", false);

    int headerLength = 30;

    Z80Header head;

    file.readBytes((byte*)&head, headerLength);

    if (head.PC == 0x00)
    {

        Z80Header2 head2;
        
        file.readBytes((byte*)&head2.Size, 2);
        file.readBytes((byte*)&head2.PC, 2);

        head.PC = head2.PC;

        headerLength += head2.Size + 2;
    }

    bool compressed = head.Info1 & 32;

    Z80TransferHeader th;

    th.I = head.I;
    th.HLP = head.HLP;
    th.DEP = head.DEP;
    th.BCP = head.BCP;
    th.AFP = head.AFP;
    th.HL = head.HL;
    th.DE = head.DE;
    th.BC = head.BC;
    th.IY = head.IY;
    th.IX = head.IX;
    th.IFFStatus = head.IFF1;
    th.R = (byte)((head.R & 0x7F) | ((head.Info1 & 0x80) << 7));
    th.AF = head.AF;
    th._SP = head._SP;
    th.IntMode = (byte)(head.Info2 & 3);
    th.BorderColor = (byte)((head.Info1 >> 1) & 7);
    th.PC = head.PC;

    buffer.seek(0);
    buffer.write((byte*)&th, sizeof(Z80TransferHeader));

    if (headerLength != 30)
    {

        compressed = true;

        dword i = headerLength;

        while (i < file.size())
        {
            dword datalen = toWord(file, i);
            word page = getPage(file.read());

            Serial.println(page);

            Serial.println(datalen);

            //if (page == 0xFFFF)
            //    return false;

            i = i + 3; // skip block header

            if (datalen == 0xFFFF)
            {
                datalen = 16384;
                compressed = false;
            }

            if(page != 0xFFFF)
                unpackMem(page, i, i + datalen, compressed, file, buffer);

            i += datalen;
        }
    }
    else
        unpackMem(0x4000, headerLength, file.size(), compressed, file, buffer);

    return true;

}

word toWord(SDFile file, word offset)
{
    file.seek(offset);
    return (word)(file.read() | (file.read() << 8));
}

word getPage(byte page)
{
    Serial.println("REQUESTED PAGE");
    Serial.println(page);

    switch (page)
    {
    case 4: return 0x8000;
    case 5: return 0xc000;
    case 8: return 0x4000;
    default: return 0xFFFF;
    }
}

void unpackMem(word offset, word start, word end, bool compressed, SDFile file, SDFile buffer)
{

    file.seek(start);
    buffer.seek(offset);

    byte tBuf[512];
    word bufPos = 0;

    for (word i = start; i < end; ++i)
    {
        if (compressed)
        {
            if (file.read() == 0x00 &&
                file.read() == 0xED &&
                file.read() == 0xED &&
                file.read() == 0x00)
                break;
            else
                file.seek(i);
        }
        
        if (compressed && file.read() == 0xED && file.read() == 0xED)
        {
            byte repeat = file.read();
            byte value = file.read();

            while (repeat-- > 0)
            {
                tBuf[bufPos++] = value;

                if (bufPos >= 512)
                {
                    buffer.write(tBuf, bufPos);
                    bufPos = 0;
                }
            }

            i = i + 3;
        }
        else
        {
            
            file.seek(i);

            tBuf[bufPos++] = (byte)file.read();

            if (bufPos >= 512)
            {
                buffer.write(tBuf, bufPos);
                bufPos = 0;
            }
        }
    }

    if (bufPos != 0)
        buffer.write(tBuf, bufPos);
}

void loadSNA(SDFile File)
{

    loadProgram(PROG_LOAD);

    bool finished = false;
    bool first = true;

    showMessage("Loading...", false);

    dword writeAddress = 16384;

    File.readBytes((byte*)header, 27);

    *status = 0x00;

    byte isLast = 0;

    while (!finished)
    {

        if (!first)
        {
            while (*status != 1);
        }

        dword segmentLength = min(256, 65536 - writeAddress);
        *segmentDest = writeAddress;
        *segmentSize = segmentLength;

        noInterrupts();
        File.readBytes((byte*)virtualSegment, segmentLength);
        interrupts();

        writeAddress += segmentLength;

        isLast = writeAddress >= 65536;

        if (first)
        {
            vramDisabled = false;
            first = false;
            *status = 0x00;
            ZXShield::EnableROMWithNMI();
        }
        else
        {
            if (isLast)	//Was this the last segment?
            {
                finished = true;

                *status = 0x55;			//Signal the program that it has to copy a segment
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
                while (*status != 1);	//Wait until the program returns to the wait loop
                *status = 0xAA;			//Signal the program that we have finished
            }
            else
            {
                *status = 0x55;
                while (*status != 0);	//Wait until the program has read the flag to avoid race conditions
            }
        }
    }

    while (!vramDisabled);

    File.close();

    showMessage("Snapshot loaded");
}

const char* getExt(const char* filename) 
{
    const char* dot = strrchr(filename, '.');

    if (!dot || dot == filename)
    {
        Serial.println(dot);

        return "";
    }
    return dot + 1;
}

void showCurrentBrowserPage()
{
    
    screen->fillScreen(ST7735_BLUE);
    screen->drawRect(0, 0, 128, 160, ST7735_CYAN);
    screen->drawRect(2, 2, 124, 156, ST7735_CYAN);

    word x = 4;
    word y = 4;

    screen->setTextColor(ST7735_CYAN, ST7735_BLUE);
    screen->setCursor(x, y);

    if (browser->IsFirstPage() && !browser->IsRoot())
    {
        browserDots = true;
        screen->println("...");
    }
    else
        browserDots = false;

    const SDBrowserEntry* page = browser->Page();

    browserCurrentPageSize = 0;

    for (int buc = 0; buc < BROWSER_PAGE_SIZE; buc++)
    {
        if (page[buc].name[0] == 0)
            break;

        y = screen->getCursorY();
        screen->setCursor(x, y);
        printBrowserEntry(buc, false);
        
        browserCurrentPageSize++;
    }

    selectedBroswserEntry();
}

void deselectBrowserEntry()
{
    screen->fillRect(4, 4 + (browserRecordIndex * 8), 120, 8, ST7735_BLUE);
    screen->setCursor(4, 4 + (browserRecordIndex * 8));

    screen->setTextColor(ST7735_CYAN, ST7735_BLUE);

    char realIndex = browserRecordIndex;

    if (browserDots)
        realIndex--;

    const SDBrowserEntry* page = browser->Page();

    if (realIndex == -1)
        screen->println("...");
    else
        printBrowserEntry(realIndex, false);
}

void selectedBroswserEntry()
{
    screen->fillRect(4, 4 + (browserRecordIndex * 8), 120, 8, ST7735_CYAN);
    screen->setCursor(4, 4 + (browserRecordIndex * 8));

    screen->setTextColor(ST7735_BLUE, ST7735_CYAN);

    char realIndex = browserRecordIndex;

    if (browserDots)
        realIndex--;

    const SDBrowserEntry* page = browser->Page();

    if (realIndex == -1)
        screen->println("...");
    else
        printBrowserEntry(realIndex, true);
}

void printBrowserEntry(byte Index, bool Selected)
{
    if(Selected)
        screen->setTextColor(ST7735_BLUE, ST7735_CYAN);
    else
        screen->setTextColor(ST7735_CYAN, ST7735_BLUE);

    const SDBrowserEntry* page = browser->Page();

    char entry[21];
    memset(entry, ' ', 20);
    entry[20] = 0;

    int len = strlen(page[Index].name);
    memcpy(entry, page[Index].name, len);

    if (!page[Index].isDir)
    {
        strlwr(entry); 

        char sizeText[7];
        memset(sizeText, 0, 7);
        
        dword len = page[Index].size;

        if (len > (dword)1024 * (dword)1024 * (dword)1024)
        {
            double val = (double)len / (1024.0 * 1024.0 * 1024.0);
            len = round(val);
            itoa(len, sizeText, 10);
            len = strlen(sizeText);
            sizeText[len++] = 'G';
            sizeText[len++] = 'b';
            sizeText[len++] = 0;
            memcpy(&entry[21 - len], sizeText, len);
        }
        else if (len > (dword)1024 * (dword)1024)
        {
            double val = (double)len / (1024.0 * 1024.0);
            len = round(val);
            itoa(len, sizeText, 10);
            len = strlen(sizeText);
            sizeText[len++] = 'M';
            sizeText[len++] = 'b';
            sizeText[len++] = 0;
            memcpy(&entry[21 - len], sizeText, len);

        }
        else if (len > (dword)1024)
        {
            double val = (double)len / 1024.0;
            len = round(val);
            itoa(len, sizeText, 10);
            len = strlen(sizeText);
            sizeText[len++] = 'K';
            sizeText[len++] = 'b';
            sizeText[len++] = 0;
            memcpy(&entry[21 - len], sizeText, len);
        }
        else
        {
            itoa(len, sizeText, 10);
            len = strlen(sizeText);
            sizeText[len++] = 'B';
            sizeText[len++] = 0;
            memcpy(&entry[21 - len], sizeText, len);
        }
    }
    else
    {
        strupr(entry);
        entry[15] = '<';
        entry[16] = 'D';
        entry[17] = 'I';
        entry[18] = 'R';
        entry[19] = '>';        
    }

    screen->println(entry);
}

void loadProgram(byte program)
{

    memset((byte*)virtualRAM, 0, 4096);

    switch (program)
    {
    case PROG_POKE:

        exitAddress = POKE_EXIT_ADDRESS;
        memcpy_P((byte*)virtualRAM, POKEProgram, POKE_ROM_LENGTH);

        break;

    case PROG_SAVE:

        header = &virtualRAM[DUMP_HEADER_ADDRESS];
        status = &virtualRAM[DUMP_STATUS_ADDRESS];
        segmentDest = (volatile word* volatile)&virtualRAM[DUMP_SEGMENT_SRC_ADDRESS];
        segmentSize = (volatile word* volatile)&virtualRAM[DUMP_SEGMENT_SIZE_ADDRESS];
        virtualSegment = &virtualRAM[DUMP_SEGMENT_ADDRESS];
        exitAddress = DUMP_EXIT_ADDRESS;

        memcpy_P((void*)virtualRAM, DUMPProgram, DUMP_SEGMENT_ADDRESS);

        break;

    case PROG_LOAD:

        header = &virtualRAM[SNA_HEADER_ADDRESS];
        status = &virtualRAM[SNA_STATUS_ADDRESS];
        startAddress = (volatile word* volatile)&virtualRAM[SNA_START_ADDRESS_ADDRESS];
        segmentDest = (volatile word* volatile)&virtualRAM[SNA_SEGMENT_DEST_ADDRESS];
        segmentSize = (volatile word* volatile)&virtualRAM[SNA_SEGMENT_SIZE_ADDRESS];
        virtualSegment = &virtualRAM[SNA_SEGMENT_ADDRESS];
        exitAddress = SNA_EXIT_ADDRESS;

        memcpy_P((void*)virtualRAM, SNAProgram, SNA_SEGMENT_ADDRESS);

        break;

    case PROG_Z80:

        //Create pointers to special virtual RAM addresses
        header = &virtualRAM[Z80_HEADER_ADDRESS];
        status = &virtualRAM[Z80_STATUS_ADDRESS];
        startAddress = (volatile word* volatile)&virtualRAM[Z80_START_ADDRESS_ADDRESS];
        segmentDest = (volatile word* volatile)&virtualRAM[Z80_SEGMENT_DEST_ADDRESS];
        segmentSize = (volatile word* volatile)&virtualRAM[Z80_SEGMENT_SIZE_ADDRESS];
        virtualSegment = &virtualRAM[Z80_SEGMENT_ADDRESS];
        exitAddress = Z80_EXIT_ADDRESS;

        memcpy_P((void*)virtualRAM, Z80Program, Z80_SEGMENT_ADDRESS);

        break;

    }
}

void RAMHandler(word Address, byte Operation)
{
    if (Address < 4096)
    {

        if (Operation)
            virtualRAM[Address] = ZXShield::InputByte();//Store data
        else
        {
            if (Address == exitAddress) //Did the speccy read the last instruction?
            {
                //disable vRAM
                vramDisabled = true;
                ZXShield::DisableROM();
            }

            ZXShield::OutputROMByte(virtualRAM[Address]); //send data

        }
    }
    else
    {
        Address = (Address - 4096) + (*sramPage * 8192);

        if (Operation)
            sram->writeByte(Address, ZXShield::InputByte());
        else
            ZXShield::OutputROMByte(sram->readByte(Address)); //send data

    }
}

void loop() {
  
    if (ACTION_SELECTED)
    {
        while (ACTION_SELECTED)
        {
            delay(10);
        }

        switch (appPage)
        {
        case INTRO_PAGE:

            appPage = 1;
            showMenu();
            break;

        case MAIN_PAGE:

            appPage = systemMenu.currentEntry + 2;

            if (appPage == POKEADOR_PAGE)
            {
                launchPokeador();
                appPage = MAIN_PAGE;
                showMenu();
            }
            else if (appPage == BROWSER_PAGE)
            {
                showBrowser();
                appPage = MAIN_PAGE;
                showMenu();
            }
            else
                showSnapshotSelector();

            break;

        default:

            if (currentSnapshot == 16)
            {
                appPage = 1;
                showMenu();
            }
            else
                chooseSnapshot();

            break;
        }
    }
    else if (ACTION_UP)
    {
        while (ACTION_UP)
        {
            delay(10);
        }
        if (appPage == 1)
            previousOption();
        else if (appPage > 1)
            previousSnapshot();
        
    }
    else if (ACTION_DOWN)
    {
        while (ACTION_DOWN)
        {
            delay(10);
        }
        if (appPage == 1)
            nextOption();
        else if (appPage > 1)
            nextSnapshot();
        
    }

}
