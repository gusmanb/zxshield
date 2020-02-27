#pragma once
#include "Arduino.h"

typedef struct _Menu
{
	const char* menuTitle;
	const char* menuEntries[10];
	char menuEntriesCount;
	char currentEntry;
} Menu, *PMenu;