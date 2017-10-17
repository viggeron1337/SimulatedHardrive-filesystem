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

std::string FileSystem::cat(std::string fileName)
{
	std::string tempName = ""; 
	char blockNr; 
	std::string fileContent = ""; 

	for (int i = 1; i < this->VHDD[currentBlock].size(); i++)
	{
		if (this->VHDD[currentBlock].getCharAt(i) != ':')
		{
			tempName += this->VHDD[currentBlock].getCharAt(i);
		}
		else
		{
			if (tempName == fileName)
			{
				blockNr = this->VHDD[currentBlock].getCharAt(i + 1); 
				i = 50000; 
			}
			else
			{
				tempName.clear();
				i++;
			}
		}
	}
	int blockNrI = atoi(&blockNr); 
	for (int i = 1; i <= this->VHDD[blockNrI].getCharInUse(); i++)
	{
		fileContent += this->VHDD[blockNrI].getCharAt(i); 
	}
	fileContent += "\n";
	return fileContent; 
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
			if (tempBlock.getCharAt(i) != ':')
			{
				blockInfo += tempBlock.getCharAt(i);
			}
			else
			{
				if (i < 510 && tempBlock.getCharAt(i + 1) != ':')
				{
					blockInfo += tempBlock.getCharAt(i);
				}
				else
				{
					i = 511;
				}
			}
			
		}
		VHDD[continueBlock].reset(); 
		continueBlock = tempBlock.getCharAt(511);
		
		if (continueBlock != ':')
		{
			tempBlock = VHDD.readBlock(continueBlock);
		}
	}
	blockInfo += appendInfo;
	VHDD.append(blockNr, blockInfo);
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
		append(fileInfo, currentBlock);
	}
}

void FileSystem::createFolder(const std::string &strName)
{
	int blockNr = this->VHDD.createDirectory(); 
	if (blockNr != -1)
	{
		std::string folderInfo = strName + ":" + std::to_string(blockNr); 
		append(folderInfo, currentBlock);
	}
}

int FileSystem::goToFolder(std::string folderName, std::string& resultName)
{
	//Fix so that cd can see maps if they are in another block than the current. 
	int found = -1; 
	std::string blockContent = ""; 
	std::string tempName = ""; 
	resultName = "/";
	for (int i = 1; i < this->VHDD[currentBlock].size(); i++)
	{
		if (this->VHDD[currentBlock].getCharAt(i) != ':')
		{
			tempName += this->VHDD[currentBlock].getCharAt(i);
		}
		else
		{
			if (tempName == folderName && this->VHDD[this->VHDD[currentBlock].getCharAt(i + 1)].getCharAt(0) == '2')
			{
				currentBlock = this->VHDD[currentBlock].getCharAt(i + 1);
				found = currentBlock;
				i = this->VHDD[currentBlock].size();	
			}
			else
			{
				tempName.clear(); 
				i++; 
			}
		}
	}
	resultName += tempName; 
	return found; 
}

/* Please insert your code */