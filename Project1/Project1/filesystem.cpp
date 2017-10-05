#include "filesystem.h"

FileSystem::FileSystem()
{
	format();
}

FileSystem::~FileSystem()
{
}

void FileSystem::format() 
{
	VHDD = MemBlockDevice();
	pos = 0;
	//createFolder();
}

void FileSystem::createFile(std::string& writeString)
{
	this->VHDD.writeFile(writeString); 
}

/* Please insert your code */