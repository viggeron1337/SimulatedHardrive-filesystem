#ifndef MEMBLOCKDEVICE_H
#define MEMBLOCKDEVICE_H
#include "blockdevice.h"

class MemBlockDevice: public BlockDevice
{
public:
    MemBlockDevice(int nrOfBlocks = 250);
    MemBlockDevice(const MemBlockDevice &other);

    ~MemBlockDevice();

    /* Operators */
    MemBlockDevice& operator=(const MemBlockDevice &other);
    Block &operator[] (int index) const;

    /* Returns amount of free blocks */
    int spaceLeft() const;

    /* Writes a block */
	int writeFile(const std::string &strBlock);

    /* Reads a block */
    Block readBlock(int blockNr) const;

    /* Resets all the blocks */
    void reset();

    /* Return the size */
    int size() const;
};

#endif // MEMBLOCKDEVICE_H
