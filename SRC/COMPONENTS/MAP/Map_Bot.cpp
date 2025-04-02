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

    int heuristicScore = 0, nLines = 0;
    uint16_t state = EMPTYLINE;
    int8_t disToTop = 30;

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        uint16_t line = (map[i] & FULLMASK(REALWIDTH));
        if (i >= Y && i < Y + BLOCK_EDGE) line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
        line &= FULLMASK(REALWIDTH);

        int nErorr = 0;
        for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
            if (getBit(state, j) && !getBit(state, j - 1)) {
                heuristicScore -= MASK(nErorr) * nErorr;
                nErorr++;
            }
            if (getBit(state, j) && !getBit(line, j))  // hole
                heuristicScore -= MASK(11);
        }

        // if (line != EMPTYLINE) // too high!
        //     heuristicScore -= 100000/((i*3+0.1) * 100/(i*3+0.1));

        state |= line;
        
        for (int j = NUMOFFSET; j < OFFRIGHT; j++)
            if (getBit(state, j)) {
                if (disToTop >= i) { disToTop = i; }
                else heuristicScore -= MASK((int)((i - disToTop)/2.0));
                heuristicScore -= MASK(7 - abs(7-j)) * (1/(disToTop + 1.1f));
            }

        if (line == FULLMASK(REALWIDTH)) nLines++;
    }

    if (nLines == 4) heuristicScore += MASK(10);
    else if (nLines >= 1) {
        heuristicScore += (nLines - 1) * MASK((int)(nLines*1.2f));
        heuristicScore -= MASK((int)(disToTop/2.5f));
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
    std::cout << maxScore << '\n';
    curBlock->rotate(timeRotate); // -22 + 4
    curBlock->updateShadow(this);
}