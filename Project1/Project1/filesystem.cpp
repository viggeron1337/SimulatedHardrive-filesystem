#include "filesystem.h"

FileSystem::FileSystem()
{
	formant();
}

FileSystem::~FileSystem()
{

}

void FileSystem::formant() 
{
	VHDD = MemBlockDevice();
	pos = 0;
	//createFolder();
}

/* Please insert your code */