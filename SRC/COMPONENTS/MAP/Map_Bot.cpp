#include "Map_Bot.hpp"

#include "CommonMap.hpp"
#include "Block.hpp"
#include "CurrentBlock.hpp"


Map_Bot::Map_Bot():Map_Multiplayer() {}

Map_Bot::~Map_Bot(){}

int Map_Bot::getScore(CurrentBlock* curBlock) {
    int state = 0, x = 0, y = 0;
    // uint16_t shape = curBlock->block->getShape();
    return 0;
}