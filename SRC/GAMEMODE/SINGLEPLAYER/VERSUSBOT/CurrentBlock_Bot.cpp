#include "CurrentBlock_Bot.hpp"

#include "Block.hpp"

CurrentBlock_Bot::CurrentBlock_Bot():CurrentBlock(){}

CurrentBlock_Bot::~CurrentBlock_Bot(){}

uint16_t CurrentBlock_Bot::getShape(int times) {
    return block->getRotate(times);
}

void CurrentBlock_Bot::setState(int stateID) {
    block->setStateID(stateID);
}