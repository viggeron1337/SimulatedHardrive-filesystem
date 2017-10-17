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

void FileSystem::createFile(std::string& writeString, int blockNr)
{

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

void FileSystem::createFolder(std::string& content, int blockNr)
{

}

std::string FileSystem::goToFolder(std::string folderName)
{
	std::string tempName = ""; 
	std::string resultName = "/"; 
	for (int i = 1; i < this->VHDD[currentBlock].size(); i++)
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
	return resultName; 
}

void FileSystem::createImage(std::string imageName)
{
	int nrOfBlocks = VHDD.getNrOfDirectories();
	int addedBlocks = 0;
	fileSystem = new FileSystem();
	std::ofstream infile;
	for (int i = 0; i < VHDD.size() && addedBlocks < nrOfBlocks; i++)
	{

	}
}

void FileSystem::remove(std::string name)
{
	std::string tempName = "";
	std::string oldFolderContent;
	std::string newFolderContent;
	char block;
	for (int i = 1; i < oldFolderContent.size(); i++)
	{
		if (oldFolderContent.at(i) != ':')
		{
			tempName += this->VHDD[currentBlock].getCharAt(i);
		}
		else
		{
			if (tempName == name)
			{
				block = this->VHDD[currentBlock].getCharAt(i + 1);
				char type = this->VHDD[block].getCharAt(0);
				if (type == '1')
				{
					removeFile(block);
				}
				else if (type == '2')
				{
					removeFolder(block);
				}
				i = this->VHDD[currentBlock].size();
			}
			else
			{
				tempName.clear();
				i++;
			}
		}
	}
}

void FileSystem::removeFile(char block)
{
	std::vector<char> blocks;
	blocks.push_back(block);
	char continueBlock = block;

	while (continueBlock != ':')
	{
		continueBlock = VHDD[continueBlock].getCharAt(511);
		if (continueBlock != ':')
		{
			blocks.push_back(continueBlock);
		}
	}
	for (int i = 0; i < blocks.size(); i++)
	{
		VHDD[blocks.at(i)].reset();
	}
}

void FileSystem::removeFolder(char block)
{

}
/* Please insert your code */