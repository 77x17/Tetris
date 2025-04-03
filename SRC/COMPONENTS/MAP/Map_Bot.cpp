#include "Map_Bot.hpp"

#include "CommonMap.hpp"
#include "Common.hpp"
#include "Block.hpp"
#include "CurrentBlock_Bot.hpp"


Map_Bot::Map_Bot():Map_Multiplayer() {}

Map_Bot::~Map_Bot(){}

#include <iostream>
#include <unistd.h>

int Map_Bot::getHeuristicScore(uint16_t shape, int X, int Y) {
    if (!isValid(shape, Y, X)) return INT_MIN;

    while (isValid(shape, Y + 1, X)) Y++;

    uint16_t state = EMPTYLINE;
    const int NWEIGHT = 6;

    int weight[NWEIGHT] = {-300, -7000, -100, -150, 300, 1000};
    int val[NWEIGHT] = {0};

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        uint16_t line = (map[i] & FULLMASK(REALWIDTH));
        if (i >= Y && i < Y + BLOCK_EDGE) line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
        line &= FULLMASK(REALWIDTH);

        for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
            if (getBit(state, j) && !getBit(state, j - 1)) {
                val[0]++;
            }
            if (getBit(state, j) && !getBit(line, j))
                val[1]++;
        }

        state |= line;

        val[2] += __builtin_popcount((state >> NUMOFFSET) & FULLMASK(10));
        val[3] += __builtin_popcount(((((state >> NUMOFFSET) & FULLMASK(10)) | ((state >> NUMOFFSET) & FULLMASK(10)>>1)) ^ ((state >> NUMOFFSET) & FULLMASK(10) & ((state >> NUMOFFSET) & FULLMASK(10)>>1))));
        val[4] += (__builtin_popcount((state & FULLMASK(REALWIDTH))) == REALWIDTH - 1);
        val[5] += (line == FULLMASK(REALWIDTH));
    }

    int heuristicScore = 0;
    for (int i = 0; i < NWEIGHT; i++) {
        heuristicScore += weight[i] * val[i];
        // std::cout << weight[i] << " " << val[i] << "\n";
        // sleep(1);
    }

    return heuristicScore;
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