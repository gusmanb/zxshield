#include "SnapshotBrowser.h"

SnapshotBrowser::SnapshotBrowser(S25FL208K* Flash, Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton)
{
    memset(snapshotStatus, 0, SNAPSHOT_PAGE_SIZE);
    flash = Flash;
    screen = Screen;
    scrW = Width;
    scrH = Height;
    txtScale = TextSize;
    upBtn = UpButton;
    downBtn = DownButton;
    selBtn = SelectButton;
}

bool SnapshotBrowser::Show()
{
    currentSnapshot = 0;

    readSnapshotStatus();

    screen->setCursor(0, 0);
    screen->fillScreen(ST7735_BLACK);
    screen->setCursor(0, 0);

    screen->setTextColor(ST7735_WHITE, ST7735_BLACK);

    int xPos = (scrW / 2) - ((15 * TXT_W * txtScale) / 2);

    screen->setCursor(xPos, 1);
    screen->println("Choose snapshot");

    int spX = scrW / 4;
    int spY = scrH / 4;

    spSize = min(spX, spY);

    yBase = scrH - ((spSize * 4) + (TXT_H * txtScale) * 2);

    pageSqrt = sqrt(SNAPSHOT_PAGE_SIZE);

    for (int x = 0; x < pageSqrt; x++)
    {
        for (int y = 0; y < pageSqrt; y++)
        {

            word color = 0;

            byte currentSnapshot = (y * pageSqrt) + x;

            if (snapshotStatus[currentSnapshot] == 1)
                color = ST7735_RED;
            else
                color = ST7735_CYAN;

            word xCoord = x * spSize;
            word yCoord = y * spSize + yBase;

            screen->fillRect(xCoord + 2, yCoord + 2, spSize - 4, spSize - 4, color);

        }
    }

    int yPos = scrH - ((TXT_H * txtScale) * 3) / 2;
    xPos = (scrW / 2) - ((6 * TXT_W * txtScale) / 2);

    screen->fillRect(0, yPos - 1, scrW, (TXT_H + 1) * txtScale, ST7735_RED);
    screen->setCursor(xPos, yPos);
    screen->setTextColor(ST7735_GREEN, ST7735_RED);
    screen->println("CANCEL");

    showSelectedSnapshot();

    while (true)
    {
        if (ACT_SELECTED)
        {
            while (ACT_SELECTED)
            {
                delay(10);
            }

            if (currentSnapshot == SNAPSHOT_PAGE_SIZE)
                return false;

            return true;
        }
        else if (ACT_UP)
        {
            while (ACT_UP)
            {
                delay(10);
            }

            previousSnapshot();

        }
        else if (ACT_DOWN)
        {
            while (ACT_DOWN)
            {
                delay(10);
            }

            nextSnapshot();

        }
    }

}

void SnapshotBrowser::readSnapshotStatus()
{
    for (int buc = 0; buc < SNAPSHOT_PAGE_SIZE; buc++)
    {
        dword currentSector = buc;
        currentSector = currentSector * 64;
        currentSector = currentSector * 1024;

        byte value;

        bool res = flash->readData(currentSector, 1, &value);

        if (!res || value != 0xFF)
            snapshotStatus[buc] = 1;
        else
            snapshotStatus[buc] = 0;
    }
}

void SnapshotBrowser::showSelectedSnapshot()
{
    
    if (currentSnapshot == SNAPSHOT_PAGE_SIZE)
    {
        int yPos = scrH - ((TXT_H * txtScale) * 3) / 2;
        int xPos = (scrW / 2) - ((6 * TXT_W * txtScale) / 2);
        screen->fillRect(0, yPos - 1, scrW, (TXT_H + 1) * txtScale, ST7735_GREEN);
        screen->setCursor(xPos, yPos);
        screen->setTextColor(ST7735_RED, ST7735_GREEN);
        screen->println("CANCEL");
    }
    else
    {
        int x = currentSnapshot % pageSqrt;
        int y = (currentSnapshot - x) / pageSqrt;

        word xCoord = x * spSize;
        word yCoord = y * spSize + yBase;

        screen->drawRect(xCoord, yCoord, spSize, spSize, ST7735_YELLOW);
    }
}

void SnapshotBrowser::clearSelectedSnapshot()
{
    if (currentSnapshot == SNAPSHOT_PAGE_SIZE)
    {
        int yPos = scrH - ((TXT_H * txtScale) * 3) / 2;
        int xPos = (scrW / 2) - ((6 * TXT_W * txtScale) / 2);
        screen->fillRect(0, yPos - 1, scrW, (TXT_H + 1) * txtScale, ST7735_RED);
        screen->setCursor(xPos, yPos);
        screen->setTextColor(ST7735_GREEN, ST7735_RED);
        screen->println("CANCEL");
    }
    else
    {
        int x = currentSnapshot % pageSqrt;
        int y = (currentSnapshot - x) / pageSqrt;

        word xCoord = x * spSize;
        word yCoord = y * spSize + yBase;

        screen->drawRect(xCoord, yCoord, spSize, spSize, ST7735_BLACK);
    }
}

void SnapshotBrowser::previousSnapshot()
{
    clearSelectedSnapshot();

    currentSnapshot--;

    if (currentSnapshot > SNAPSHOT_PAGE_SIZE)
        currentSnapshot = SNAPSHOT_PAGE_SIZE;

    showSelectedSnapshot();
}

void SnapshotBrowser::nextSnapshot()
{
    clearSelectedSnapshot();

    currentSnapshot++;

    if (currentSnapshot > SNAPSHOT_PAGE_SIZE)
        currentSnapshot = 0;

    showSelectedSnapshot();
}