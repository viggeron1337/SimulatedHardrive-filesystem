#ifndef BLOCKDEVICE_H
#define BLOCKDEVICE_H

/*
 * Pure virtual class
 */

#include "block.h"

class BlockDevice
{
protected:
    Block* memBlocks;
    int nrOfBlocks;
    int freePointer;
public:
    BlockDevice(int nrOfBlocks);
    BlockDevice(const BlockDevice &other);

    virtual ~BlockDevice();
    virtual int spaceLeft() const = 0;
    virtual int writeFile(const std::string &strBlock) = 0;
    virtual Block readBlock(int blockNr) const = 0;
    virtual void reset() = 0;
    virtual int size() const = 0;
};

#endif // BLOCKDEVICE_H
