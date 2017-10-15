#include "filesystem.h"

FileSystem::FileSystem()
{
	format();
}

FileSystem::~FileSystem()
{
}

std::string  FileSystem::listDirectory()
{
	std::string blockInfo;
	Block tempBlock = VHDD.readBlock(this->currentBlock);
	char continueBlock = '0'; 
	while (continueBlock != ':')
	{
		for (int i = 1; i < 511; i++)
		{
			blockInfo += tempBlock.getCharAt(i);
			if (i >= 2 && blockInfo.at(i - 1) == ':' && blockInfo.at(i - 2) == ':')
			{
				i = 511;
			}
		}
		continueBlock = tempBlock.getCharAt(511); 
		if (continueBlock != ':')
		{
			tempBlock = VHDD.readBlock(continueBlock); 
		}
	}

	std::string cleanBlockInfo; 
	for (int i = 0; i < blockInfo.size(); i++)
	{
		if (blockInfo.at(i) != ':')
		{
			cleanBlockInfo += blockInfo.at(i); 
		}
		else
		{
			cleanBlockInfo += "\n"; 
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

	while (continueBlock != ':')
	{
		for (int i = 1; i < 511; i++)
		{
			blockInfo += tempBlock.getCharAt(i);
		}
		VHDD[continueBlock].reset(); 
		continueBlock = tempBlock.getCharAt(511);
		
		if (continueBlock != ':')
		{
			tempBlock = VHDD.readBlock(continueBlock);
		}
	}
	blockInfo += appendInfo;
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

std::string FileSystem::goToFolder(std::string folderName)
{
	std::string tempName = ""; 
	for (int i = 0; i < this->VHDD[currentBlock].size; i++)
	{
		if (this->VHDD[currentBlock].getCharAt(i) != ':')
		{
			tempName += this->VHDD[currentBlock].getCharAt(i);
		}
		else
		{
			if (tempName == folderName)
			{
				currentBlock = this->VHDD[currentBlock].getCharAt(i + 1); 
				i == this->VHDD[currentBlock].size; 
			}
			else
			{
				tempName.clear(); 
				i++; 
			}
		}
	}
	return tempName; 
}

/* Please insert your code */