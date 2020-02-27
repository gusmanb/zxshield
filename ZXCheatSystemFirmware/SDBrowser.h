#pragma once

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

typedef unsigned long dword;

typedef struct SDBrowserEntry
{
	bool isDir;
	char name[13];
	dword size;
};

class SDBrowser
{

public:
	SDBrowser(byte SDCs, dword Speed, byte PageSize);
	bool open();
	bool close();
	bool nextPage();
	bool previousPage();
	bool openRoot();
	bool openFolder(byte Index);
	bool openFolder(const char* Path);
	bool parentFolder();
	SDFile openFile(byte Index, char* PathOutput = NULL);
	SDFile openFile(const char* Path);
	SDFile createFile(const char* Path);
	bool deleteFile(const char* Path);

	const SDBrowserEntry* Page() const { return page; }
	byte PageSize() const { return pageSize; }
	byte CurrentPageSize() const { return currentPageSize; }
	const char* CurrentPath() const { return currentPath;  }
	const char* CurrentFolder() const { return currentEntry.name(); }
	bool IsFirstPage() { return pageStartIndex == 0; }
	bool IsLastPage() { return page[pageSize-1].name[0] == 0; }
	bool IsRoot() { return !strcmp(currentPath, "/"); }

private:
	word pageStartIndex = 0;
	char currentPath[256];
	SDFile currentEntry;
	bool isOpen = false;
	byte pageSize = 0;
	byte currentPageSize = 0;
	SDBrowserEntry* page;
	byte mCspin;
	dword speed;

};

