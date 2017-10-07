#ifndef MEMBLOCKDEVICE_H
#define MEMBLOCKDEVICE_H
#include "blockdevice.h"

class MemBlockDevice: public BlockDevice
{
private:
	int nrOfDirectories = 0;
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

	/* Writes a directory */
	int createDirectory(); 

    /* Reads a block */
    Block readBlock(int blockNr) const;

    /* Resets all the blocks */
    void reset();

    /* Return the size */
    int size() const;
};

#endif // MEMBLOCKDEVICE_H
