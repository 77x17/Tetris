#ifndef BLOCKFACTORY_HPP
#define BLOCKFACTORY_HPP

#include <random>

class Block;
class BlockElement;

class BlockFactory {
public:
    static Block* createRandomBlock();
    static Block* getBlockType(int x);
    static void createBag(BlockElement* &head, BlockElement* &tail, std::mt19937 &gen);
};

#endif