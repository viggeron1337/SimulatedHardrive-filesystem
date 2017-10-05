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
    int output = -1;    // Assume blockNr out-of-range
	float nrOfBlocksF = strBlock.size() / 510; 
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
	if (foundBlocks == nrOfBlocksI)
	{
		output = 1; 
		for (int i = 0; i < nrOfBlocksI - 1; i++)
		{
			this->memBlocks[blockNeededArray[i]].writeBlock(strBlock.substr(510 * i, 510),'1',blockNeededArray[i + 1]); 
		}
		//This makes sure that the last char is -1 (standard value) for the last block. 
		this->memBlocks[blockNeededArray[nrOfBlocksI]].writeBlock(strBlock.substr(510 * nrOfBlocksI, 510), '1'); 
	}
	delete[] blockNeededArray; 
    return output;
}

Block MemBlockDevice::readBlock(int blockNr) const {
    if (blockNr < 0 || blockNr >= this->nrOfBlocks)
        throw std::out_of_range("Block out of range");
    else {
        Block a(this->memBlocks[blockNr]);
        return a;
    }
}

/* Resets all the blocks */
void MemBlockDevice::reset() {
    for (int i = 0; i < this->nrOfBlocks; ++i) {
        this->memBlocks[i].reset('0');
    }
}

int MemBlockDevice::size() const {
    return this->nrOfBlocks;
}
