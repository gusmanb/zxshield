#pragma once
#include "Arduino.h"
#include <Adafruit_ST7735.h>
#include "DialogHelper.h"
#include "SDPagedFileSystem.h"

typedef void (*FileSelectedCallback)(SDFile File);

#define ACT_SELECTED  (!digitalRead(selBtn))
#define ACT_UP       (!digitalRead(upBtn))
#define ACT_DOWN     (!digitalRead(downBtn))

#define TXT_W 6
#define TXT_H 8

class SDBrowser
{
public:
	SDBrowser(SDPagedFileSystem* FileSystem, DialogHelper* Dialog, Adafruit_ST7735* Screen,word Width, word Height, word TextSize, byte UpButton, byte DownButton, byte SelectButton);
	SDFile Show(FileSelectedCallback Callback, const char* AllowedExtensions[], int ExtensionCount);
private:
	void doBrowserActionUp();
	void doBrowserActionDown();
	void showCurrentBrowserPage();
	void deselectBrowserEntry();
	void selectedBroswserEntry();
	void printBrowserEntry(byte Index, bool Selected);
	bool extensionAllowed(const char* fileName, const char* extensions[], int extNumber);
	const char* getExt(const char* filename);

	Adafruit_ST7735* screen;
	DialogHelper* dialog;
	SDPagedFileSystem* fileSystem;
	word scrW;
	word scrH;
	word txtScale;
	byte upBtn;
	byte downBtn;
	byte selBtn;
	byte currentSnapshot;
	byte maxEntries;
	byte lineWidth;

	char recordIndex = 0;
	byte currentPageSize = 0;
	bool hasDots = false;
};

