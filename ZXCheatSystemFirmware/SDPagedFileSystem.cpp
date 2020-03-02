#include "SDPagedFileSystem.h"

SDPagedFileSystem::SDPagedFileSystem(byte SDCs, dword Speed)
{
	mCspin = SDCs;
	speed = Speed;

	pinMode(mCspin, OUTPUT);
	digitalWrite(mCspin, HIGH);
}

void SDPagedFileSystem::SetPageSize(byte PageSize)
{
	pageSize = PageSize;
	page = (SDEntry*)malloc(sizeof(SDEntry) * PageSize);
}

bool SDPagedFileSystem::open()
{
	if (isOpen)
		return false;

	isOpen = SD.begin(speed, mCspin);

	if (isOpen)
		openRoot();

	return isOpen;
}

bool SDPagedFileSystem::close()
{
	if (!isOpen)
		return false;

	currentEntry.close();
	SD.end();
	isOpen = false;

	pinMode(mCspin, OUTPUT);
	digitalWrite(mCspin, HIGH);

	return true;
}

bool SDPagedFileSystem::nextPage()
{
	if (!isOpen)
		return false;

	if (page[pageSize - 1].name[0] == 0)
		return false;

	SDFile file = currentEntry.openNextFile();

	if (!file)
		return false;

	memset(page, 0, sizeof(SDEntry) * pageSize);

	currentPageSize = 0;

	for (int buc = 0; buc < pageSize; buc++)
	{
		if (!file)
			break;

		currentPageSize++;

		memcpy(page[buc].name, file.name(), 12);
		page[buc].isDir = file.isDirectory();
		page[buc].size = file.size();
		file.close();

		file = currentEntry.openNextFile();
	}

	if (file)
		file.close();

	pageStartIndex += pageSize;

	return true;
}

bool SDPagedFileSystem::previousPage()
{
	if (!isOpen || pageStartIndex == 0)
		return false;

	if (page[pageSize - 1].name[0] == 0)
		return false;

	currentEntry.rewindDirectory();

	pageStartIndex -= pageSize;

	SDFile file;

	for (int buc = 0; buc < pageStartIndex; buc++)
	{
		file = currentEntry.openNextFile();
		file.close();
	}

	memset(page, 0, sizeof(SDEntry) * pageSize);

	currentPageSize = 0;

	for (int buc = 0; buc < pageSize; buc++)
	{
		file = currentEntry.openNextFile();

		if (!file)
			break;

		currentPageSize++;

		memcpy(page[buc].name, file.name(), 12);
		page[buc].isDir = file.isDirectory();
		page[buc].size = file.size();
		file.close();
	}

	return true;
}

bool SDPagedFileSystem::openRoot()
{
	if (!isOpen)
		return false;

	memset(currentPath, 0, 256);
	strcpy(currentPath, "/");

	if (currentEntry)
		currentEntry.close();

	currentEntry = SD.open("/");

	pageStartIndex = 0;

	SDFile file;

	memset(page, 0, sizeof(SDEntry) * pageSize);

	currentPageSize = 0;

	for (int buc = 0; buc < pageSize; buc++)
	{
		file = currentEntry.openNextFile();

		if (!file)
			break;

		currentPageSize++;

		memcpy(page[buc].name, file.name(), 12);
		page[buc].isDir = file.isDirectory();
		page[buc].size = file.size();
		file.close();
	}

	return true;
}

bool SDPagedFileSystem::openFolder(byte Index)
{
	if (!isOpen || Index >= pageSize)
		return false;

	SDEntry* entry = &page[Index];

	if (!entry->isDir)
		return false;

	if(strcmp(currentPath, "/"))
		strcat(currentPath, "/");

	strcat(currentPath, entry->name);

	if (currentEntry)
		currentEntry.close();

	currentEntry = SD.open(currentPath);

	memset(page, 0, sizeof(SDEntry) * pageSize);
	pageStartIndex = 0;

	SDFile file;

	currentPageSize = 0;

	for (int buc = 0; buc < pageSize; buc++)
	{
		file = currentEntry.openNextFile();

		if (!file)
			break;

		currentPageSize++;

		memcpy(page[buc].name, file.name(), 12);
		page[buc].isDir = file.isDirectory();
		page[buc].size = file.size();
		file.close();
	}

	return true;
}

bool SDPagedFileSystem::openFolder(const char* Path)
{
	if (!isOpen)
		return false;

	if (currentEntry)
		currentEntry.close();

	currentEntry = SD.open(Path);

	if (!currentEntry || !currentEntry.isDirectory())
		return false;

	memset(currentPath, 0, 256);
	strcpy(currentPath, Path);

	memset(page, 0, sizeof(SDEntry) * pageSize);
	pageStartIndex = 0;

	SDFile file;

	currentPageSize = 0;

	for (int buc = 0; buc < pageSize; buc++)
	{
		file = currentEntry.openNextFile();

		if (!file)
			break;

		currentPageSize++;

		memcpy(page[buc].name, file.name(), 12);
		page[buc].isDir = file.isDirectory();
		page[buc].size = file.size();

		file.close();
	}

	return true;
}

bool SDPagedFileSystem::parentFolder()
{
	if (!isOpen || !strcmp(currentPath, "/"))
		return false;

	char* prev = strrchr(currentPath, '/');
	*prev = 0;

	memset(page, 0, sizeof(SDEntry) * pageSize);
	pageStartIndex = 0;

	if (currentEntry)
		currentEntry.close();

	currentEntry = SD.open(currentPath);

	SDFile file;

	currentPageSize = 0;

	for (int buc = 0; buc < pageSize; buc++)
	{
		file = currentEntry.openNextFile();

		if (!file)
			break;

		currentPageSize++;

		memcpy(page[buc].name, file.name(), 12);
		page[buc].isDir = file.isDirectory();
		page[buc].size = file.size();

		file.close();
	}

	return true;
}

SDFile SDPagedFileSystem::openFile(byte Index, char* PathOutput = NULL)
{
	if (!isOpen || Index >= pageSize)
		return SDFile();

	if (strcmp(currentPath, "/"))
		strcat(currentPath, "/");

	word pos = strlen(currentPath);

	if (pos != 1)
		pos -= 1;

	strcat(currentPath, page[Index].name);

	SDFile file = SD.open(currentPath);

	if (PathOutput != NULL)
		strcpy(PathOutput, currentPath);

	currentPath[pos] = 0;

	return file;
}

SDFile SDPagedFileSystem::openFile(const char* Path)
{
	return SD.open(Path);
}

SDFile SDPagedFileSystem::createFile(const char* Path)
{
	return SD.open(Path, O_RDWR | O_CREAT);
}

bool SDPagedFileSystem::deleteFile(const char* Path)
{
	return SD.remove(Path);
}