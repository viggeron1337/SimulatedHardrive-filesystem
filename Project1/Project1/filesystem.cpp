#include "filesystem.h"

FileSystem::FileSystem()
{
	format();
}

FileSystem::FileSystem(std::string imageName)
{
	restoreImage(imageName);
}

FileSystem::~FileSystem()
{
}

std::string  FileSystem::listDirectory()
{
	std::string blockInfo;
	Block tempBlock = VHDD.readBlock(atoi(&currentBlock));
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
			tempBlock = VHDD.readBlock(atoi(&continueBlock));
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
			int blockNr = 0; 
			cleanBlockInfo += "		"; 
			blockNr = atoi(&blockInfo.at(i + 1)); 

			if (blockNr != 0)
			{
				if (this->VHDD[blockNr].getCharAt(0) == '1')
				{
					cleanBlockInfo += "File";
				}
				else if (this->VHDD[blockNr].getCharAt(0) == '2')
				{
					cleanBlockInfo += "Directory";
				}
				cleanBlockInfo += "\n";
			}
			i++; 
		}
	}
	return cleanBlockInfo; 
}

int FileSystem::cat(std::string fileName, std::string& fileContent)
{
	std::string tempName = ""; 
	int found = -1; 
	char blockNrC; 
	int blockNrI = 0; 

	for (int i = 1; i < this->VHDD[atoi(&currentBlock)].size(); i++)
	{
		if (this->VHDD[atoi(&currentBlock)].getCharAt(i) != ':')
		{
			tempName += this->VHDD[atoi(&currentBlock)].getCharAt(i);
		}
		else
		{
			if (tempName == fileName)
			{
				blockNrC = this->VHDD[atoi(&currentBlock)].getCharAt(i + 1);
				blockNrI = atoi(&blockNrC); 
				if (this->VHDD[blockNrI].getCharAt(0) == '1')
				{
					found = 1; 
					i = this->VHDD[atoi(&currentBlock)].size();
				}
				else
				{
					tempName.clear(); 
					i++; 
					blockNrI = 0;
				}
			}
			else
			{
				tempName.clear();
				i++;
			}
		}
	}
	if (found != -1)
	{
		for (int i = 1; i <= this->VHDD[blockNrI].getCharInUse(); i++)
		{
			fileContent += this->VHDD[blockNrI].getCharAt(i);
		}
		fileContent += "\n";
	}
	return found; 
}

void FileSystem::append(std::string & appendInfo, char blockNr)
{
	if (blockNr == -1)
	{
		blockNr = currentBlock;
	}
	char type = VHDD[atoi(&blockNr)].getCharAt(0); 
	Block tempBlock = VHDD.readBlock(atoi(&blockNr));
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
	VHDD.append(atoi(&blockNr), blockInfo);
}

void FileSystem::format() 
{
	VHDD = MemBlockDevice();
	currentBlock = '0';
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
	VHDD.writeFile(writeString, blockNr);
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
	VHDD.createDirectory(content, blockNr);
}

int FileSystem::goToFolder(std::string folderName, std::string& resultName)
{
	int found = -1; 
	if (folderName == "/")
	{
		found = 0;
		resultName = "/."; 
		currentBlock = '0'; 
	}
	else
	{
		std::string tempName = "";
		resultName = "/";
		for (int i = 1; i < this->VHDD[atoi(&currentBlock)].size(); i++)
		{
			if (this->VHDD[atoi(&currentBlock)].getCharAt(i) != ':')
			{
				tempName += this->VHDD[atoi(&currentBlock)].getCharAt(i);
			}
			else
			{

				if (tempName == folderName)
				{
					char newBlock = this->VHDD[atoi(&currentBlock)].getCharAt(i + 1);
					if (this->VHDD[atoi(&newBlock)].getCharAt(0) == '2')
					{
						currentBlock = newBlock;
						found = atoi(&currentBlock);
						i = this->VHDD[atoi(&currentBlock)].size();
					}
					else
					{
						tempName.clear();
						i++;
					}
				}
				else
				{
					tempName.clear();
					i++;
				}
			}
		}
		resultName += tempName;
	}
	return found;
}

void FileSystem::createImage(std::string imageName)
{
	int nrOfBlocks = VHDD.getNrOfDirectories();
	int addedBlocks = 0;
	std::ofstream outfile;
	outfile.open(imageName);
	std::string content;
	outfile << nrOfBlocks << std::endl;
	for (int i = 0; addedBlocks < nrOfBlocks; i++)
	{
		if (this->VHDD[i].getCharAt(0) != '0')
		{
			content = "";
			if (this->VHDD[i].getCharInUse() == 0)
			{
				content += this->VHDD[i].getCharAt(0);
			}
			else
			{
				for (int k = 0; k < this->VHDD[i].getCharInUse() + 1; k++)
				{
					content += this->VHDD[i].getCharAt(k);
				}
			}
			outfile << i << std::endl << content << std::endl;
			addedBlocks++;
		}
	}
	outfile.close();
}
void FileSystem::restoreImage(std::string imageName)
{
	VHDD = MemBlockDevice();
	currentBlock = '0';
	std::string strBlock = "";
	int intBlock;
	int nrOfBlocks;
	std::string str;
	std::ifstream infile;
	if (imageName.size() <= 4 || imageName.substr(imageName.size() - 4, 4) != ".txt")
	{
		imageName += ".txt";
	}
	infile.open(imageName);
	infile >> nrOfBlocks;
	infile.ignore();
	for (int i = 0; i < nrOfBlocks; i++)
	{
		std::getline(infile, strBlock);
		std::getline(infile, str);
		intBlock = atoi(&strBlock.at(0));
		if (str.at(0) == '1')
		{
			createFile(str, intBlock);
		}
		else if (str.at(0) == '2')
		{
			createFolder(str, intBlock);
		}
	}
	infile.close();
}

void FileSystem::remove(std::string name)
{
	std::string tempName = "";
	std::string oldFolderContent; 
	std::string newFolderContent;
	char block = '-1'; 

	for (int i = 0; i < this->VHDD[atoi(&currentBlock)].size(); i++)
	{
		oldFolderContent += this->VHDD[atoi(&currentBlock)].getCharAt(i);
	}

	for (int i = 1; i < oldFolderContent.size(); i++)
	{
		if (oldFolderContent.at(i) != ':')
		{
			tempName += this->VHDD[atoi(&currentBlock)].getCharAt(i);
		}
		else
		{
			if (tempName == name)
			{
				block = this->VHDD[atoi(&currentBlock)].getCharAt(i + 1);
				char type = this->VHDD[atoi(&block)].getCharAt(0);
				if (type == '1')
				{
					removeFile(block);
					i = this->VHDD[atoi(&currentBlock)].size();
				}
				//Inte implementerad.
				else if (type == '2')
				{
					//removeFolder(block);
					//Eftersom removeFolder inte är ordentligt implementerad så har vi 
					// denna check för att återställa block.
					block = '-1'; 
					tempName.clear();
					i++;
				}
				//är inflytad i ifsatsen tills removeFolder är fixad
				//i = this->VHDD[atoi(&currentBlock)].size();
			}
			else
			{
				tempName.clear();
				i++;
			}
		}
	}
	if (block != '-1')
	{
		std::string tempName = ""; 
		newFolderContent = oldFolderContent.at(0);
		for (int i = 1; i < oldFolderContent.size(); i++)
		{
			if (oldFolderContent.at(i) != ':')
			{
				tempName += oldFolderContent.at(i); 
			}
			else
			{
				if (tempName == "")
				{
					i = oldFolderContent.size();
				}
				else if (tempName != name)
				{
					newFolderContent += tempName;
					newFolderContent += ':';
					newFolderContent += oldFolderContent.at(++i); 
					tempName.clear();
				}
				else
				{
					block = this->VHDD[atoi(&currentBlock)].getCharAt(i + 1);
					char type = this->VHDD[atoi(&block)].getCharAt(0);
					if (type == '1')
					{
						tempName.clear();
						i++;
					}
					else if(type == '2')
					{
						newFolderContent += tempName;
						newFolderContent += ':';
						newFolderContent += oldFolderContent.at(++i);
						tempName.clear();
					}
					
				}
			}
		}
		std::vector<int> blocks;
		blocks.push_back(atoi(&currentBlock));
		char continueBlock = currentBlock; 

		while (continueBlock != ':')
		{
			continueBlock = VHDD[atoi(&continueBlock)].getCharAt(511);
			if (continueBlock != ':')
			{
				blocks.push_back(continueBlock);
			}
		}
		for (int i = 0; i < blocks.size(); i++)
		{
			VHDD[blocks.at(i)].reset();
			this->VHDD.createDirectory(newFolderContent.substr(510 * i,510), blocks.at(i));
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
		continueBlock = VHDD[atoi(&continueBlock)].getCharAt(511);
		if (continueBlock != ':')
		{
			blocks.push_back(continueBlock);
		}
	}
	for (int i = 0; i < blocks.size(); i++)
	{
		VHDD[blocks.at(i)].reset();
	}
	VHDD.changeNrOfDirectoriesWith((blocks.size() * -1));
}

void FileSystem::removeFolder(char block)
{

}

void FileSystem::cp(std::string originalName, std::string copysName)
{
	std::string tempName;
	for (int i = 1; i < this->VHDD[atoi(&currentBlock)].size(); i++)
	{
		if (this->VHDD[atoi(&currentBlock)].getCharAt(i) != ':')
		{
			tempName += this->VHDD[atoi(&currentBlock)].getCharAt(i);
		}
		else
		{

			if (tempName == originalName)
			{
				char newBlock = this->VHDD[atoi(&currentBlock)].getCharAt(i + 1);
				if (this->VHDD[atoi(&newBlock)].getCharAt(0) == '1')
				{
					char continueBlock = newBlock;
					std::string content = "";
					while (continueBlock != ':')
					{
						for (int i = 1; i <= this->VHDD[atoi(&newBlock)].getCharInUse(); i++)
						{
							content += this->VHDD[atoi(&continueBlock)].getCharAt(i);
							if (i >= 2 && content.at(i - 1) == ':' && content.at(i - 2) == ':')
							{
								i = 511;
							}
						}
						continueBlock = this->VHDD[atoi(&continueBlock)].getCharAt(511);
					}
					createFile(copysName, content);
				}
				else
				{
					tempName.clear();
					i++;
				}
			}
			else
			{
				tempName.clear();
				i++;
			}
		}
	}
}
/* Please insert your code */