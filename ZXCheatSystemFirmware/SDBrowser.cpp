#include "SDBrowser.h"

SDBrowser::SDBrowser(SDPagedFileSystem* FileSystem, DialogHelper* Dialog, Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton)
{
    maxEntries = (Height - 16) / (TXT_H * TextSize);
    lineWidth = (Width - 8) / (TXT_W * TextSize);

    fileSystem = FileSystem;
    fileSystem->SetPageSize(maxEntries);
    dialog = Dialog;
    screen = Screen;
    scrW = Width;
    scrH = Height;
    txtScale = TextSize;
    upBtn = UpButton;
    downBtn = DownButton;
    selBtn = SelectButton;
}

SDFile SDBrowser::Show(FileSelectedCallback Callback, const char* AllowedExtensions[], int ExtensionCount)
{
    screen->fillScreen(ST7735_BLACK);
    dialog->ShowMessage("Opening card...", false);

    if (!fileSystem->open())
    {
        dialog->ShowMessage("Error opening card");
        fileSystem->close();
        return SDFile();
    }

    recordIndex = 0;

    showCurrentBrowserPage();

    while (true)
    {
        if (ACT_SELECTED)
        {
            while (ACT_SELECTED)
            {
                if (ACT_DOWN)
                {
                    while (ACT_DOWN || ACT_SELECTED);
                    return SDFile();
                }
            }

            if (hasDots)
                recordIndex--;

            if (recordIndex == -1)
            {
                fileSystem->parentFolder();
                recordIndex = 0;
                showCurrentBrowserPage();
            }
            else
            {
                if (fileSystem->Page()[recordIndex].isDir)
                {
                    fileSystem->openFolder(recordIndex);
                    recordIndex = 0;
                    showCurrentBrowserPage();
                }
                else
                {

                    if (!extensionAllowed(fileSystem->Page()[recordIndex].name, AllowedExtensions, ExtensionCount))
                    {
                        dialog->ShowMessage("Invalid file");
                        showCurrentBrowserPage();
                    }
                    else
                    {
                        SDFile selectedFile = fileSystem->openFile(recordIndex);
                        Callback(selectedFile);
                        recordIndex = 0;
                        fileSystem->close();
                        return;
                    }
                }
            }
        }
        else if (ACT_UP)
        {
            int delayup = 0;
            int waitDelay = 1000;

            while (ACT_UP)
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

            if (waitDelay == 1000)
                doBrowserActionUp();

        }
        else if (ACT_DOWN)
        {
            int delaydown = 0;
            int waitDelay = 1000;

            while (ACT_DOWN)
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

void SDBrowser::doBrowserActionUp()
{
    if (recordIndex == 0)
    {
        if (!fileSystem->IsFirstPage())
        {
            fileSystem->previousPage();

            if (fileSystem->IsFirstPage() && !fileSystem->IsRoot())
                recordIndex = maxEntries;
            else
                recordIndex = maxEntries - 1;

            showCurrentBrowserPage();
        }
    }
    else
    {
        deselectBrowserEntry();
        recordIndex--;
        selectedBroswserEntry();
    }
}

void SDBrowser::doBrowserActionDown()
{

    if ((hasDots && recordIndex == fileSystem->CurrentPageSize()) ||
        (!hasDots && recordIndex == fileSystem->CurrentPageSize() - 1))
    {
        if (!fileSystem->IsLastPage())
        {
            recordIndex = 0;
            fileSystem->nextPage();
            showCurrentBrowserPage();
        }
    }
    else
    {
        deselectBrowserEntry();
        recordIndex++;
        selectedBroswserEntry();

    }

}

void SDBrowser::showCurrentBrowserPage()
{

    screen->fillScreen(ST7735_BLUE);
    screen->drawRect(0, 0, scrW, scrH, ST7735_CYAN);
    screen->drawRect(2, 2, scrW - 4, scrH - 4, ST7735_CYAN);

    word x = 4;
    word y = 4;

    screen->setTextColor(ST7735_CYAN, ST7735_BLUE);
    screen->setCursor(x, y);

    if (fileSystem->IsFirstPage() && !fileSystem->IsRoot())
    {
        hasDots = true;
        screen->println("...");
    }
    else
        hasDots = false;

    const SDEntry* page = fileSystem->Page();

    currentPageSize = 0;

    for (int buc = 0; buc < maxEntries; buc++)
    {
        if (page[buc].name[0] == 0)
            break;

        y = screen->getCursorY();
        screen->setCursor(x, y);
        printBrowserEntry(buc, false);

        currentPageSize++;
    }

    selectedBroswserEntry();
}

void SDBrowser::deselectBrowserEntry()
{
    screen->fillRect(4, 4 + (recordIndex * 8), scrW - 8, 8, ST7735_BLUE);
    screen->setCursor(4, 4 + (recordIndex * 8));

    screen->setTextColor(ST7735_CYAN, ST7735_BLUE);

    char realIndex = recordIndex;

    if (hasDots)
        realIndex--;

    if (realIndex == -1)
        screen->println("...");
    else
        printBrowserEntry(realIndex, false);
}

void SDBrowser::selectedBroswserEntry()
{
    screen->fillRect(4, 4 + (recordIndex * 8), scrW - 8, 8, ST7735_CYAN);
    screen->setCursor(4, 4 + (recordIndex * 8));

    screen->setTextColor(ST7735_BLUE, ST7735_CYAN);

    char realIndex = recordIndex;

    if (hasDots)
        realIndex--;

    if (realIndex == -1)
        screen->println("...");
    else
        printBrowserEntry(realIndex, true);
}

void SDBrowser::printBrowserEntry(byte Index, bool Selected)
{
    if (Selected)
        screen->setTextColor(ST7735_BLUE, ST7735_CYAN);
    else
        screen->setTextColor(ST7735_CYAN, ST7735_BLUE);

    const SDEntry* page = fileSystem->Page();

    char* entry = (char*)malloc(lineWidth + 1);
    memset(entry, ' ', lineWidth);
    entry[lineWidth] = 0;

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
            memcpy(&entry[lineWidth - len], sizeText, len);
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
            memcpy(&entry[lineWidth - len], sizeText, len);

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
            memcpy(&entry[lineWidth - len], sizeText, len);
        }
        else
        {
            itoa(len, sizeText, 10);
            len = strlen(sizeText);
            sizeText[len++] = 'B';
            sizeText[len++] = 0;
            memcpy(&entry[lineWidth - len], sizeText, len);
        }

    }
    else
    {
        strupr(entry);
        entry[lineWidth - 5] = '<';
        entry[lineWidth - 4] = 'D';
        entry[lineWidth - 3] = 'I';
        entry[lineWidth - 2] = 'R';
        entry[lineWidth - 1] = '>';
    }

    screen->println(entry);

    free(entry);
}

bool SDBrowser::extensionAllowed(const char* fileName, const char* extensions[], int extNumber)
{
    const char* ext = getExt(fileName);

    for (int buc = 0; buc < extNumber; buc++)
        if (!strcasecmp(ext, extensions[buc]))
            return true;

    return false;
}

const char* SDBrowser::getExt(const char* filename)
{
    const char* dot = strrchr(filename, '.');

    if (!dot || dot == filename)
        return "";

    return dot + 1;
}