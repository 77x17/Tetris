#include "Map_Bot.hpp"

#include "CommonMap.hpp"
#include "Common.hpp"
#include "Block.hpp"
#include "CurrentBlock_Bot.hpp"


Map_Bot::Map_Bot():Map_Multiplayer() {}

Map_Bot::~Map_Bot(){}

int Map_Bot::getScore(uint16_t shape, int X, int Y) {
    if (!isValid(shape, Y, X)) return -1;

    int cnt = 1000000;

    while (isValid(shape, Y + 1, X)) Y++;

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < Common::HEIGHT_MAP) {
        map[Y + i] ^= (getLine(shape, i) << (X + NUMOFFSET));
    }

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        if ((map[i] & FULLMASK(REALWIDTH)) == EMPTYLINE) cnt += 20 * i;
        else if ((map[i] & FULLMASK(REALWIDTH)) == FULLMASK(REALWIDTH)) cnt += 1000;
        else cnt -= (REALWIDTH - __builtin_popcount(map[i] & FULLMASK(REALWIDTH))) * i;
    }
    
    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < Common::HEIGHT_MAP) {
        map[Y + i] ^= (getLine(shape, i) << (X + NUMOFFSET));
    }
    return cnt;
}

#include <iostream>

void Map_Bot::findPath(int8_t &X, CurrentBlock_Bot* curBlock) {
    uint8_t stateID = 0;
    int maxScore = 0;

    for (int i = -2; i <= REALWIDTH; i++)
        for (int j = 0; j < 4; j++) {
            int tmp = getScore(curBlock->getShape(j), i, 0);
            if (maxScore < tmp) {
                maxScore = tmp;
                X = i; stateID = j;
            }
        }
    std::cout << (int)X << " " << (int)stateID << " " << maxScore << '\n';
    curBlock->setState(stateID); // -22 + 4
}