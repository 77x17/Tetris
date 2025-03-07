#ifndef BLOCKFACTORY_HPP
#define BLOCKFACTORY_HPP

#include <random>

class Block;
class BlockEle;

class BlockFactory {
public:
    static Block* createRandomBlock();
    static Block* getBlockType(int x);
    static void createBag(BlockEle* &head, BlockEle* &tail);
    // static int createBag(); // network
};

#endif