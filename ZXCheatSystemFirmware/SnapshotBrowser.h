#pragma once

#include "Arduino.h"
#include <Adafruit_ST7735.h>
#include "S25FL208K.h"

#define RGB_COLOR(r,g,b) (((dword)(r & 0xF8)) << 8 | ((dword)(g & 0xFC)) << 3 | ((dword)(b & 0xF8)) >> 3)

#define ACT_SELECTED (!digitalRead(selBtn))
#define ACT_UP       (!digitalRead(upBtn))
#define ACT_DOWN     (!digitalRead(downBtn))

#define TXT_W 6
#define TXT_H 8

#define SNAPSHOT_PAGE_SIZE 16

class SnapshotBrowser
{
public:
	SnapshotBrowser(S25FL208K* Flash, Adafruit_ST7735* Screen, word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton);
	bool Show();
	byte SelectedSnapshot() { return currentSnapshot; }
	bool IsEmpty() { return !snapshotStatus[currentSnapshot]; }

private:
	void nextSnapshot();
	void previousSnapshot();
	void showSelectedSnapshot();
	void clearSelectedSnapshot();
	void readSnapshotStatus();
	S25FL208K* flash;
	Adafruit_ST7735* screen;
	word scrW;
	word scrH;
	word txtScale;
	byte upBtn;
	byte downBtn;
	byte selBtn;
	byte currentSnapshot;
	bool isEmpty;
	byte snapshotStatus[SNAPSHOT_PAGE_SIZE];
	word spSize;
	word yBase;
	word pageSqrt;
};

