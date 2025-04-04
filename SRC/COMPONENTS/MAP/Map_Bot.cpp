#include "Map_Bot.hpp"

#include "CommonMap.hpp"
#include "Common.hpp"
#include "Block.hpp"
#include "CurrentBlock_Bot.hpp"
#include "BotBrain.hpp"

Map_Bot::Map_Bot():Map_Multiplayer() {
    botBrain = new BotBrain(map);
}

Map_Bot::~Map_Bot(){
    delete botBrain;
}

void Map_Bot::reset() {
    Map::reset();
    botBrain->updateWeight();
}

#include <iostream>
#include <unistd.h>

int Map_Bot::getHeuristicScore(uint16_t shape, int X, int Y) {
    if (!isValid(shape, Y, X)) return INT_MIN;

    while (isValid(shape, Y + 1, X)) Y++;
    return botBrain->getHeuristicScore(shape, X, Y);
}

void Map_Bot::findPath(int8_t &X, CurrentBlock_Bot* curBlock) {
    uint8_t timeRotate = 0;
    int maxScore = INT_MIN;
    int MID = Common::WIDTH_MAP / 2 - BLOCK_EDGE / 2;

    for (int i = 0; i <= 5; i++) {
        for (int j = 0; j < 4; j++) for (int t: {-1, 1}) {
            int tmp = getHeuristicScore(curBlock->getShape(j), MID + i*t, 0);
            if (maxScore < tmp) {
                maxScore = tmp;
                X = MID + i*t; timeRotate = j;
            }
        }
    }
    // std::cout << maxScore << '\n';
    curBlock->rotate(timeRotate); // -22 + 4
    curBlock->updateShadow(this);
}