#pragma once
#include "Arduino.h"
#include <Adafruit_ST7735.h>

#define RGB_COLOR(r,g,b) (((dword)(r & 0xF8)) << 8 | ((dword)(g & 0xFC)) << 3 | ((dword)(b & 0xF8)) >> 3)

#define MENU_SELECTED (!digitalRead(selBtn))
#define MENU_UP       (!digitalRead(upBtn))
#define MENU_DOWN     (!digitalRead(downBtn))

#define TXT_W 6
#define TXT_H 8

typedef struct _Menu
{
	const char* menuTitle;
	const char* menuEntries[10];
	char menuEntriesCount;
	char currentEntry;

} Menu, *PMenu;

class MenuSystem
{
public:
	MenuSystem(Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton);
	byte ShowMenu(PMenu Menu);
private:
	void renderMenu(PMenu menu);
	void nextOption(PMenu menu);
	void previousOption(PMenu menu);
	void printMenuEntry(PMenu menu, byte entryNumber, bool selected);
	Adafruit_ST7735* screen;
	word scrW;
	word scrH;
	word txtScale;
	byte upBtn;
	byte downBtn;
	byte selBtn;
};