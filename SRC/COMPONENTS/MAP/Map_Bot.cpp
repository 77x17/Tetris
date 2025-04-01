#include "Map_Bot.hpp"

#include "CommonMap.hpp"
#include "Common.hpp"
#include "Block.hpp"
#include "CurrentBlock_Bot.hpp"


Map_Bot::Map_Bot():Map_Multiplayer() {}

Map_Bot::~Map_Bot(){}

#include <iostream>
#include <unistd.h>

int Map_Bot::getScore(uint16_t shape, int X, int Y) {
    if (!isValid(shape, Y, X)) return -1;

    int cnt = 1000000;

    while (isValid(shape, Y + 1, X)) Y++;
    uint16_t prevLine = EMPTYLINE;

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        uint16_t line = (map[i] & FULLMASK(REALWIDTH));
        if (i >= Y && i < Y + BLOCK_EDGE) line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
        line &= FULLMASK(REALWIDTH);

        for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
            if (getBit(line, j) && !getBit(line, j - 1))
                cnt -= i;
            if (getBit(prevLine, j) && !getBit(line, j))
                cnt -= 100;
        }

        if (line == FULLMASK(REALWIDTH)) cnt += 1000;

        prevLine = line;
    }
    return cnt;
}

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