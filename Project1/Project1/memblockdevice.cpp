#include "memblockdevice.h"
#include <stdexcept>

MemBlockDevice::MemBlockDevice(int nrOfBlocks): BlockDevice(nrOfBlocks) {

}

MemBlockDevice::MemBlockDevice(const MemBlockDevice &other) : BlockDevice(other) {

}

MemBlockDevice::~MemBlockDevice() {
    /* Implicit call to base-class destructor */
}

MemBlockDevice& MemBlockDevice::operator=(const MemBlockDevice &other) {
    delete [] this->memBlocks;
    this->nrOfBlocks = other.nrOfBlocks;
    this->freePointer = other.freePointer;
    this->memBlocks = new Block[this->nrOfBlocks];

    for (int i = 0; i < this->nrOfBlocks; ++i)
        this->memBlocks[i] = other.memBlocks[i];

    return *this;
}

Block& MemBlockDevice::operator[](int index) const {
    if (index < 0 || index >= this->nrOfBlocks) {
        throw std::out_of_range("Illegal access\n");
    }
    else {
        return this->memBlocks[index];
    }
}

int MemBlockDevice::spaceLeft() const {
    /* Not yet implemented */
    return 0;
}

int MemBlockDevice::writeFile(const std::string &strBlock) {
    int blockNr = -1;    // Assume blockNr out-of-range
	int size = strBlock.size();
	float nrOfBlocksF = (float)strBlock.size() / 510; 
	int nrOfBlocksI = nrOfBlocksF; 
	
	if (nrOfBlocksF != nrOfBlocksI)
	{
		nrOfBlocksI++;
	}
	int* blockNeededArray = new int[nrOfBlocksI]; 
	int foundBlocks = 0; 

	for (int i = 0; i < this->nrOfBlocks && foundBlocks < nrOfBlocksI; i++)
	{
		//Inserts any data to be stored onto the first empty block. 
		if (this->memBlocks[i].getCharAt(0) == '0')
		{
			blockNeededArray[foundBlocks++] = i;
		}
	}
	//Makes sure that we add the data if there was blocks to support it. 
	if (nrOfBlocksI == 1)
	{
		this->memBlocks[blockNeededArray[0]].writeBlock(strBlock, '1');
	}
	else
	{
		if (foundBlocks == nrOfBlocksI)
		{
			blockNr = blockNeededArray[0];
			for (int i = 0; i < nrOfBlocksI - 1; i++)
			{
				this->memBlocks[blockNeededArray[i]].writeBlock(strBlock.substr(510 * i, 510), '1', blockNeededArray[i + 1]);
				size - 510;
			}
			//This makes sure that the last char is : (standard value) for the last block. 
			this->memBlocks[blockNeededArray[nrOfBlocksI]].writeBlock(strBlock.substr(510 * nrOfBlocksI, size), '1'); //fixa
		}
	}
	delete[] blockNeededArray; 
    return blockNr;
}

int MemBlockDevice::createDirectory()
{
	int emptyPos = -1; 
	for (int i = 0; i < this->nrOfBlocks && emptyPos == -1; i++)
	{
		if (this->memBlocks[i].getCharAt(0) == '0')
		{
			this->memBlocks[i].writeBlock("", '2');
			emptyPos = i; 
		}
	}
	return emptyPos; 
}

void MemBlockDevice::appendDirectory(std::string &appendIfon, int appendedBlock)
{
	int blockNr = appendedBlock;
	int size = appendIfon.size();
	float nrOfBlocksF = (float)appendIfon.size() / 510;
	int nrOfBlocksI = nrOfBlocksF;

	if (nrOfBlocksF != nrOfBlocksI)
	{
		nrOfBlocksI++;
	}
	int* blockNeededArray = new int[nrOfBlocksI];
	int foundBlocks = 1;
	blockNeededArray[0] = appendedBlock;
	for (int i = 0; i < this->nrOfBlocks && foundBlocks < nrOfBlocksI; i++)
	{
		//Inserts any data to be stored onto the first empty block. 
		if (this->memBlocks[i].getCharAt(0) == '0')
		{
			blockNeededArray[foundBlocks++] = i;
		}
	}
	//Makes sure that we add the data if there was blocks to support it.
	if (nrOfBlocksI == 1)
	{
		this->memBlocks[blockNeededArray[0]].writeBlock(appendIfon, '2');
	}
	else
	{
		if (foundBlocks == nrOfBlocksI)
		{
			blockNr = blockNeededArray[0];
			for (int i = 0; i < nrOfBlocksI - 1; i++)
			{
				this->memBlocks[blockNeededArray[i]].writeBlock(appendIfon.substr(510 * i, 510), '2', blockNeededArray[i + 1]);
				size - 510;
			}
			//This makes sure that the last char is : (standard value) for the last block. 
			this->memBlocks[blockNeededArray[nrOfBlocksI]].writeBlock(appendIfon.substr(510 * nrOfBlocksI, size), '2');
		}
	}
	delete[] blockNeededArray;
}
Block MemBlockDevice::readBlock(int blockNr) const {
	if (blockNr < 0 || blockNr >= this->nrOfBlocks)
	{
		//throw std::out_of_range("Block out of range");
	}
    else {
        Block a(this->memBlocks[blockNr]);
        return a;
    }
}

/* Resets all the blocks */
void MemBlockDevice::reset() {
    for (int i = 0; i < this->nrOfBlocks; ++i) {
        this->memBlocks[i].reset();
    }
}

int MemBlockDevice::size() const {
    return this->nrOfBlocks;
}

void MemBlockDevice::append(int appendedBlock, std::string &appendIfon)
{
	float nrOfBlocksF = (float)appendIfon.size() / 510;
	int nrOfBlocksI = nrOfBlocksF;
	if (nrOfBlocksF != nrOfBlocksI)
	{
		nrOfBlocksI++;
	}
	int* blockNeededArray = new int[nrOfBlocksI];
	blockNeededArray[0] = appendedBlock;
	int nrOfBlock = 1;
	for (int i = 0; i < nrOfBlocksI; i++)
	{
		blockNeededArray[nrOfBlock] = this->memBlocks[i].getCharAt(511);
		nrOfBlock++;
	}
	
	appendDirectory(appendIfon, appendedBlock);
	//delete[] blockNeededArray;
}