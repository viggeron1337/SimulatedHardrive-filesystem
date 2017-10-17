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
	int nrOfElementsInUse = 0; 

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
	int getCharInUse() const; 

    /* Write a block */
    void writeBlock(const std::string &strBlock,char blockType, char continueBlock = ':');
    int writeBlock(const std::vector<char> &vec);
    void writeBlock(const char cArr[]);     // Use with caution! Make sure that cArr is at least as large as private member block.

    std::string toString() const;
};

#endif // BLOCK_H
