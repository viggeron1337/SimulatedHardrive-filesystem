#include "block.h"
#include <stdexcept>


Block::Block(int size) {
    /* Setting size */
    if (size > 0)
        this->nrOfElements = size;
    else
        this->nrOfElements = 512;

    this->block = new char[this->nrOfElements];

    // Sets everything to 0
    this->reset();
}

Block::Block(const Block &other) {
    this->nrOfElements = other.nrOfElements;
    this->block = new char[this->nrOfElements];
    for (int i = 0; i < this->nrOfElements; ++i)
        this->block[i] = other.block[i];
}

Block::~Block() {
    delete [] this->block;
}

Block &Block::operator =(const Block &other) {
    delete [] this->block;
    this->nrOfElements = other.nrOfElements;
    this->block = new char[this->nrOfElements];
    for (int i = 0; i < this->nrOfElements; ++i)
        this->block[i] = other.block[i];
    return *this;
}

char Block::operator[](int index) const {
    if (index < 0 || index >= this->nrOfElements) {
        throw std::out_of_range("Illegal access\n");
    }
    else {
        return this->block[index];
    }
}

void Block::reset(char c) {
    for (int i = 0; i < this->nrOfElements; ++i)
        this->block[i] = c;
}

int Block::size() const {
    return this->nrOfElements;
}

Block Block::readBlock() const {
    return Block(*this);
}

char Block::getCharAt(int pos)
{
	return this->block[pos]; 
}

void Block::writeBlock(const std::string &strBlock,char blockType, int continueBlock) {
	this->block[0] = blockType; 
       for (int i = 1; i < this->nrOfElements - 1; ++i) {
            this->block[i] = strBlock[i];
        }
	   this->block[511] = continueBlock; 
}

int Block::writeBlock(const std::vector<char> &vec) {
    int output = -2; // Assume not the same dimension
    if (vec.size() == (unsigned long)this->nrOfElements) {
        for (unsigned long int i = 0; i < vec.size(); ++i) {
           this->block[i] = vec[i];
        }
        output = 1;
    }
//    else {
//        throw std::out_of_range("vector and block not the same dimension");
//    }
    return output;
}

void Block::writeBlock(const char cArr[]) {
    for (int i = 0; i < this->nrOfElements; ++i) {
        this->block[i] = cArr[i];
    }
}

std::string Block::toString() const {
    std::string output;
    output.reserve(this->nrOfElements);
    for (int i = 0; i < this->nrOfElements; ++i)
        output += this->block[i];
    return output;
}





