#pragma once
#include "Arduino.h"
#include <Adafruit_ST7735.h>

#define RGB_COLOR(r,g,b) (((word)(r & 0xF8)) << 8 | ((word)(g & 0xFC)) << 3 | ((word)(b & 0xF8)) >> 3)

#define ACT_SELECTED  (!digitalRead(selBtn))
#define ACT_UP       (!digitalRead(upBtn))
#define ACT_DOWN     (!digitalRead(downBtn))

#define TXT_W 6
#define TXT_H 8

class DialogHelper
{
public:
	DialogHelper(Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton);
	void ShowMessage(const char* Message, bool WaitInput = true);
	bool ShowConfirm(const char* Message);
private:
	Adafruit_ST7735* screen;
	word scrW;
	word scrH;
	word txtScale;
	byte upBtn;
	byte downBtn;
	byte selBtn;
};

