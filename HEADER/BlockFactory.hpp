#ifndef BLOCKFACTORY_HPP
#define BLOCKFACTORY_HPP

#include <random>

#include "Block.hpp"

class BlockFactory {
public:
    static Block* createRandomBlock();
};

#endif