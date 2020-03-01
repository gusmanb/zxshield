#include "MenuSystem.h"

MenuSystem::MenuSystem(Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton)
{
	screen = Screen;
	scrW = Width;
	scrH = Height;
	txtScale = TextSize;
	upBtn = UpButton;
	downBtn = DownButton;
	selBtn = SelectButton;
}

byte MenuSystem::ShowMenu(PMenu Menu)
{
    screen->setTextSize(txtScale);

    renderMenu(Menu);

	while (true)
	{
        if (MENU_SELECTED)
        {
            while (MENU_SELECTED)
            {
                delay(10);
            }

            return Menu->currentEntry;
        }
        else if (MENU_UP)
        {
            while (MENU_UP)
            {
                delay(10);
            }
           
            previousOption(Menu);

        }
        else if (MENU_DOWN)
        {
            while (MENU_DOWN)
            {
                delay(10);
            }

            nextOption(Menu);

        }
	}

}

void MenuSystem::renderMenu(PMenu menu)
{
    screen->setTextColor(ST7735_CYAN, ST7735_BLUE);

    screen->fillScreen(ST7735_BLUE);
    screen->drawRect(0, 0, scrW, scrH, ST7735_CYAN);
    screen->drawRect(2, 2, scrW - 4, scrH - 4, ST7735_CYAN);

    screen->setCursor((TXT_W + 1) * txtScale, TXT_H * txtScale);
    screen->println(menu->menuTitle);
    screen->println();
    screen->println();

    for (int buc = 0; buc < menu->menuEntriesCount; buc++)
        printMenuEntry(menu, buc, buc == menu->currentEntry);

}

void MenuSystem::previousOption(PMenu menu)
{
    printMenuEntry(menu, menu->currentEntry, false);

    menu->currentEntry--;

    if (menu->currentEntry < 0)
        menu->currentEntry = menu->menuEntriesCount - 1;

    printMenuEntry(menu, menu->currentEntry, true);
}

void MenuSystem::nextOption(PMenu menu)
{
    printMenuEntry(menu, menu->currentEntry, false);

    menu->currentEntry++;

    if (menu->currentEntry >= menu->menuEntriesCount)
        menu->currentEntry = 0;

    printMenuEntry(menu, menu->currentEntry, true);
}

void MenuSystem::printMenuEntry(PMenu menu, byte entryNumber, bool selected)
{
    entryNumber * 10 + 30;

    int y = (entryNumber + 3) * (TXT_H + 2) * txtScale;

    if (selected)
    {
        screen->setTextColor(ST7735_BLUE, ST7735_CYAN);
        screen->fillRect(4, y - 1, scrW - 8, (TXT_H + 1) * txtScale, ST7735_CYAN);
    }
    else
    {
        screen->setTextColor(ST7735_CYAN, ST7735_BLUE);
        screen->fillRect(4, y - 1, scrW - 8, (TXT_H + 1) * txtScale, ST7735_BLUE);
    }

    screen->setCursor(4, y);
    screen->println(menu->menuEntries[entryNumber]);
}
