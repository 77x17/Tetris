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
                cnt -= 500;
            if (getBit(prevLine, j) && !getBit(line, j))
                cnt -= 1200;
        }

        if (line == FULLMASK(REALWIDTH)) cnt += 2000;
        if (line != EMPTYLINE) cnt -= MASK((60-i*2)/5);

        prevLine = line;
    }
    return cnt;
}

void Map_Bot::findPath(int8_t &X, CurrentBlock_Bot* curBlock) {
    uint8_t timeRotate = 0;
    int maxScore = 0;
    int MID = Common::WIDTH_MAP / 2 - BLOCK_EDGE / 2;

    for (int i = 0; i <= 5; i++) {
        for (int j = 0; j < 4; j++) for (int t: {-1, 1}) {
            int tmp = getScore(curBlock->getShape(j), MID + i*t, 0);
            if (maxScore < tmp) {
                maxScore = tmp;
                X = MID + i*t; timeRotate = j;
            }
        }
    }
    curBlock->rotate(timeRotate); // -22 + 4
    curBlock->updateShadow(this);
}