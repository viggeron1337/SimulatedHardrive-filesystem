#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <iostream>

class Block
{
private:
    char *info;
    int nrOfElements;

public:
    /* Constructor */
    Block(int nrOfElements = 512);    // overloaded (default) constructor
    Block(const Block &other); // copy-constructor

    /* Destructor */
    ~Block();

    /* Operators */
    Block& operator = (const Block &other);  // Assignment operator
    char operator[] (int index) const;  // []-operator
    friend std::ostream& operator<<(std::ostream &os, const Block& blck)
    {
        for (int i = 0; i < blck.nrOfElements; ++i)
            os << blck.info[i];
        return os;
    }

	void reset();  // Sets every element in char-array to 0
    int size() const;   // returns the size
    Block readBlock() const;    // Returns a copy of block
	char getCharAt(int pos); 

    /* Write a block */
    void writeBlock(const std::string &strBlock,char blockType, char continueBlock = ':');
    void writeBlock(const std::string &strBlock);

    std::string toString() const;
};

#endif // BLOCK_H
