#include "BotBrain.hpp"

#include "Common.hpp"
#include "CommonMap.hpp"

BotBrain::BotBrain(uint64_t* map) : map(map) {}
BotBrain::~BotBrain() {}


int BotBrain::getHeuristicScore(uint16_t shape, int X, int Y) {
    uint16_t state = EMPTYLINE;
    const int NWEIGHT = 6;

    int weight[NWEIGHT] = {-300, -7000, -100, -75, 300, 1000};
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
        val[3] += __builtin_popcount((((state | (state >> 1)) ^ (state & (state >> 1))) & FULLMASK(OFFRIGHT - 1)) >> NUMOFFSET);
        val[3] += __builtin_popcount((((state | (state << 1)) ^ (state & (state << 1))) & FULLMASK(OFFRIGHT)) >> (NUMOFFSET + 1));
        // val[3] += __builtin_popcount(((((state >> NUMOFFSET) & FULLMASK(10)) | ((state >> NUMOFFSET) & FULLMASK(10)>>1)) ^ ((state >> NUMOFFSET) & FULLMASK(10) & ((state >> NUMOFFSET) & FULLMASK(10)>>1))));
        val[4] += (__builtin_popcount((state & FULLMASK(REALWIDTH))) == REALWIDTH - 1);
        val[5] += (line == FULLMASK(REALWIDTH));
    }

    int heuristicScore = 0;
    for (int i = 0; i < NWEIGHT; i++) {
        heuristicScore += weight[i] * val[i];
    }

    return heuristicScore;
}