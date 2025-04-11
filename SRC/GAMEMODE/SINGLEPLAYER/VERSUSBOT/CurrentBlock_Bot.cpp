#include "CurrentBlock_Bot.hpp"

#include "Block.hpp"

CurrentBlock_Bot::CurrentBlock_Bot():CurrentBlock(){}
CurrentBlock_Bot::CurrentBlock_Bot(Block* p):CurrentBlock(p){}

CurrentBlock_Bot::~CurrentBlock_Bot(){}

uint8_t CurrentBlock_Bot::getShapeID() {
    return block->getShapeID();
}

uint16_t CurrentBlock_Bot::getShape(int times) {
    return block->getRotate(times);
}

void CurrentBlock_Bot::rotate(int x) {
    block->rotate(x);
}