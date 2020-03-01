#include "DialogHelper.h"

DialogHelper::DialogHelper(Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton)
{
	screen = Screen;
	scrW = Width;
	scrH = Height;
	txtScale = TextSize;
	upBtn = UpButton;
	downBtn = DownButton;
	selBtn = SelectButton;
}


void DialogHelper::ShowMessage(const char* Message, bool WaitInput = true)
{
    screen->setTextColor(ST7735_WHITE, RGB_COLOR(0, 168, 175));

    int quarter = scrH / 4;

    screen->fillRect(0, quarter, scrW, quarter * 2, RGB_COLOR(0, 168, 175));
    screen->drawRect(2, quarter + 2, scrW - 4, quarter * 2 - 4, ST7735_BLACK);
    screen->drawRect(4, quarter + 4, scrW - 8, quarter * 2 - 8, ST7735_BLACK);
 
    int xPos = (scrW / 2) - (((strlen(Message) / 2) * (TXT_W * txtScale)));
    screen->setCursor(xPos, scrH / 2 - (TXT_H * txtScale) / 2);
    screen->println(Message);

    if (WaitInput)
    {
        while (!ACT_SELECTED)
        {
            delay(10);
        }

        while (ACT_SELECTED)
        {
            delay(10);
        }
    }
}

bool DialogHelper::ShowConfirm(const char* Message)
{
    screen->setTextColor(ST7735_WHITE, RGB_COLOR(0, 168, 175));

    int quarter = scrH / 4;

    screen->fillRect(0, quarter, scrW, quarter * 2, RGB_COLOR(0, 168, 175));
    screen->drawRect(2, quarter + 2, scrW - 4, quarter * 2 - 4, ST7735_BLACK);
    screen->drawRect(4, quarter + 4, scrW - 8, quarter * 2 - 8, ST7735_BLACK);

    screen->setTextColor(ST7735_WHITE, RGB_COLOR(0, 168, 175));

    int xPos = (scrW / 2) - (((strlen(Message) / 2) * (TXT_W * txtScale)));
    int yPos = scrH / 3 + (TXT_H * txtScale) / 2;
    screen->setCursor(xPos, yPos);
    screen->println(Message);

    yPos = scrH / 2 + (TXT_W * txtScale);
    xPos = (scrW / 2) - ((8 * (TXT_W * txtScale)));

    screen->setTextColor(RGB_COLOR(0, 168, 175), ST7735_WHITE);
    screen->setCursor(xPos, yPos);
    screen->println(" -No            ");
    screen->setCursor(xPos, yPos + (TXT_H * txtScale));
    screen->setTextColor(ST7735_WHITE, RGB_COLOR(0, 168, 175));
    screen->println(" -Yes           ");

    bool answer = false;
    bool selected = false;

    while (!selected)
    {
        if (ACT_SELECTED)
        {
            while (ACT_SELECTED)
            {
                delay(10);
            }

            selected = true;
            break;
        }
        else if (ACT_UP || ACT_DOWN)
        {
            while (ACT_UP || ACT_DOWN)
            {
                delay(10);
            }

            answer = !answer;

            if (!answer)
            {
                screen->setTextColor(RGB_COLOR(0, 168, 175), ST7735_WHITE);
                screen->setCursor(xPos, yPos);
                screen->println(" -No            ");
                screen->setCursor(xPos, yPos + (TXT_H * txtScale));
                screen->setTextColor(ST7735_WHITE, RGB_COLOR(0, 168, 175));
                screen->println(" -Yes           ");
            }
            else
            {
                screen->setTextColor(ST7735_WHITE, RGB_COLOR(0, 168, 175));
                screen->setCursor(xPos, yPos);
                screen->println(" -No            ");
                screen->setCursor(xPos, yPos + (TXT_H * txtScale));
                screen->setTextColor(RGB_COLOR(0, 168, 175), ST7735_WHITE);
                screen->println(" -Yes           ");
            }
        }
    }

    return answer;
}
