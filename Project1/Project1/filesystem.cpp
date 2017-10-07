#include "filesystem.h"

FileSystem::FileSystem()
{
	format();
}

FileSystem::~FileSystem()
{
}

std::string & FileSystem::listDirectory()
{
	std::string blockInfo;
	Block tempBlock = VHDD.readBlock(this->currentBlock);
	char continueBlock = '0'; 
	while (continueBlock != '-1')
	{
		for (int i = 1; i < 511; i++)
		{
			blockInfo += tempBlock.getCharAt(i);
		}
		continueBlock = tempBlock.getCharAt(511); 
		if (continueBlock != '-1')
		{
			tempBlock = VHDD.readBlock(continueBlock); 
		}
	}

	std::string cleanBlockInfo = ""; 
	for (int i = 0; i < blockInfo.size(); i++)
	{
		if (blockInfo.at(i) != ':')
		{
			cleanBlockInfo += blockInfo.at(i); 
		}
		else
		{
			cleanBlockInfo += "/n"; 
			i++; 
		}
	}
	return cleanBlockInfo; 
}

void FileSystem::append(std::string & appendInfo, int blockNr)
{
	if (blockNr == -1)
	{
		blockNr = currentBlock;
	}
	char type = VHDD[blockNr].getCharAt(0); 
	Block tempBlock = VHDD.readBlock(blockNr);
	std::string blockInfo; 
	char continueBlock = blockNr;

	while (continueBlock != '-1')
	{
		for (int i = 1; i < 511; i++)
		{
			blockInfo += tempBlock.getCharAt(i);
		}
		VHDD[continueBlock].reset(); 
		continueBlock = tempBlock.getCharAt(511);
		
		if (continueBlock != '-1')
		{
			tempBlock = VHDD.readBlock(continueBlock);
		}
	}
	blockInfo += appendInfo;
	if()
}

void FileSystem::format() 
{
	VHDD = MemBlockDevice();
	currentBlock = 0;
	VHDD.createDirectory(); 
}

void FileSystem::createFile(const std::string& strName, std::string& writeString)
{
	int blockNr = 0; 
	blockNr = this->VHDD.writeFile(writeString); 
	if (blockNr != -1)
	{
		std::string fileInfo = strName + ":" + std::to_string(blockNr);
		//Append created blockinfo to current Block (Directory).
	}
}

void FileSystem::createFolder(const std::string &strName)
{
	int blockNr = 0; 
	blockNr = this->VHDD.createDirectory(); 
	if (blockNr != -1)
	{
		std::string folderInfo = strName + ":" + std::to_string(blockNr); 
		//Append created blockinfo to current Block (Directory).
	}
}

/* Please insert your code */