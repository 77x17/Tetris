#include "BotBrain.hpp"

#include <fstream>
#include "Common.hpp"
#include "CommonMap.hpp"
#include "iostream"

#include <random>

BotBrain::BotBrain(uint64_t* map) : map(map) {
    numWeight = 6; preSumScore = 0;
    weight = new int[numWeight]();
    preWeight = new int[numWeight]();

    std::fstream f("./ASSETS/BotBrain.txt");
    int p = 0; 
    while (p < numWeight && f >> weight[p]) {
        preWeight[p] = weight[p];
        p++;
    }
}

BotBrain::~BotBrain() {
    std::fstream f("./ASSETS/BotBrain.txt");
    int p = 0; while (p < numWeight && f << preWeight[p++]);
    delete[] weight;
    delete[] preWeight;
}

void BotBrain::setScore(uint64_t val) {
    countSet++; sumScore += val;
}


int BotBrain::getHeuristicScore(uint16_t shape, int X, int Y) {

    if (countSet >= 5) {
        countSet = 0;
        if (preSumScore != INT32_MAX) {
            for (int i = 0; i < numWeight; i++)
                preWeight[i] = (preSumScore * preWeight[i] + sumScore * weight[i]) / (sumScore + preSumScore);
            std::random_device rd;
            std::mt19937 gen(rd());
            for (int i = 0; i < numWeight; i++) 
                weight[i] = preWeight[i] + (gen() % 20 - 10);
        }
        std::cout << preSumScore << " " << sumScore << '\n';
        preSumScore = sumScore;
    }

    uint16_t state = EMPTYLINE;

    int weight[numWeight] = {-300, -7000, -100, -75, 300, 1000};
    int val[numWeight] = {0};

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
    for (int i = 0; i < numWeight; i++) {
        heuristicScore += weight[i] * val[i];
    }

    return heuristicScore;
}