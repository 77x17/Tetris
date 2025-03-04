#ifndef BLOCKFACTORY_HPP
#define BLOCKFACTORY_HPP

#include <random>

class Block; 

class BlockFactory {
public:
    static Block* createRandomBlock();
};

#endif