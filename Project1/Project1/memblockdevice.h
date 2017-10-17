#ifndef MEMBLOCKDEVICE_H
#define MEMBLOCKDEVICE_H
#include "blockdevice.h"

class MemBlockDevice: public BlockDevice
{
private:
	int nrOfDirectories = 0;
	void appendDirectory(std::string &appendIfon, int appendedBlock);
public:
    MemBlockDevice(int nrOfBlocks = 250);
    MemBlockDevice(const MemBlockDevice &other);

    ~MemBlockDevice();

    /* Operators */
    MemBlockDevice& operator=(const MemBlockDevice &other);
    Block &operator[] (int index) const;

    /* Returns amount of free blocks */
    int spaceLeft() const;

    /* Writes a file */
	int writeFile(const std::string &strBlock);
	void writeFile(const std::string &strBlock, int blockNr);
	/* Writes a directory */
	int createDirectory(); 
	void createDirectory(const std::string &strBlock, int blockNr);
    /* Reads a block */
    Block readBlock(int blockNr) const;

    /* Resets all the blocks */
    void reset();

    /* Return the size */
    int size() const;
	int getNrOfDirectories() const;
	void append(int appendedBlock, std::string &appendIfon);
};

#endif // MEMBLOCKDEVICE_H
