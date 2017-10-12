#include "block.h"
#include <stdexcept>


Block::Block(int size) {
    /* Setting size */
    if (size > 0)
        this->nrOfElements = size;
    else
        this->nrOfElements = 512;

    this->info = new char[this->nrOfElements];

    // Sets everything to 0
    this->reset();
}

Block::Block(const Block &other) {
    this->nrOfElements = other.nrOfElements;
    this->info = new char[this->nrOfElements];
    for (int i = 0; i < this->nrOfElements; ++i)
        this->info[i] = other.info[i];
}

Block::~Block() {
    delete [] this->info;
}

Block &Block::operator =(const Block &other) {
    delete [] this->info;
    this->nrOfElements = other.nrOfElements;
    this->info = new char[this->nrOfElements];
    for (int i = 0; i < this->nrOfElements; ++i)
        this->info[i] = other.info[i];
    return *this;
}

char Block::operator[](int index) const {
    if (index < 0 || index >= this->nrOfElements) {
        throw std::out_of_range("Illegal access\n");
    }
    else {
        return this->info[index];
    }
}

void Block::reset() {
	this->info[0] = '0';
	for (int i = 1; i < this->nrOfElements; ++i)
	{
		this->info[i] = ':';
	}
}

int Block::size() const {
    return this->nrOfElements;
}

Block Block::readBlock() const {
    return Block(*this);
}

char Block::getCharAt(int pos)
{
	return this->info[pos];
}

void Block::writeBlock(const std::string &strBlock,char blockType, char continueBlock) {
	this->info[0] = blockType;
       for (int i = 0; i < strBlock.size(); i++) {
            this->info[i + 1] = strBlock[i];
        }
	   this->info[511] = continueBlock;
}

int Block::writeBlock(const std::vector<char> &vec) {
    int output = -2; // Assume not the same dimension
    if (vec.size() == (unsigned long)this->nrOfElements) {
        for (unsigned long int i = 0; i < vec.size(); ++i) {
           this->info[i] = vec[i];
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
        this->info[i] = cArr[i];
    }
}

std::string Block::toString() const {
    std::string output;
    output.reserve(this->nrOfElements);
    for (int i = 0; i < this->nrOfElements; ++i)
        output += this->info[i];
    return output;
}





