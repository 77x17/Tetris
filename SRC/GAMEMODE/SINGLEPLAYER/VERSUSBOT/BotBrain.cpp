#include "BotBrain.hpp"


#include "Common.hpp"
#include "CommonMap.hpp"
#include "iostream"

#include <random>

#define getStateLine(x) (((x) >> NUMOFFSET) & FULLMASK(Common::WIDTH_MAP))
#define nBit1(x) (__builtin_popcount(x))
#define nBit0(x) (nBit1((~(x)) & FULLMASK(Common::WIDTH_MAP)))

BotBrain::BotBrain(uint64_t* map) : map(map) {
    numWeight = 9; numSpecLimmit = 4;
    // preSumScore = 0;
    weight = new int64_t* [numSpecLimmit]();
    for (int i = 0; i < numSpecLimmit; i++)
        weight[i] = new int64_t[numWeight]();


    // preWeight = new int64_t[numWeight]();
    // delta = new int64_t[numWeight]();
    // for (int i = 0; i < numWeight; i++) 
    //     delta[i] = 20;
    load();
    err.open("err.txt");
}

void BotBrain::load() {
    std::fstream f("./ASSETS/BotBrain.txt");
    
    for (int i = 0; i < numSpecLimmit; i++)
        for (int j = 0; j < numWeight; j++)
            f >> weight[i][j];

    f.close();
}

BotBrain::~BotBrain() {
    for (int i = 0; i < numSpecLimmit; i++)
        delete[] weight[i];
    delete[] weight;
}

void BotBrain::updateWeight() {
    load();
    return;

    // if (countSet < 5) return;

    // std::cout << "WEIGHT: "; for (int i= 0; i < numWeight; i++) std::cout << weight[i] << " "; 
    // std::cout << '\n';

    // for (int i = 0; i < numWeight; i++) {
    //     preWeight[i] = weight[i] * (preSumScore - sumScore) / (sumScore + preSumScore);
    //     delta[i] = delta[i] * (preSumScore - sumScore) / (sumScore + preSumScore);
    // }
    
    // std::fstream f("./ASSETS/BotBrain.txt");
    // int p = 0; while (p < numWeight && f << preWeight[p++] << " ");

    // std::cout << "PREWEIGHT: "; for (int i= 0; i < numWeight; i++) std::cout << preWeight[i] << " ";
    // std::cout << '\n';

    // std::cout << "DELTA: "; for (int i= 0; i < numWeight; i++) std::cout << delta[i] << " ";
    // std::cout << '\n';

    // // std::random_device rd;
    // // std::mt19937 gen(rd());
    // for (int i = 0; i < numWeight; i++) 
    //     weight[i] = preWeight[i] + delta[i];

    // preSumScore = sumScore;
    // countSet = 0; sumScore = 0;
}

void BotBrain::setScore(int val) {
    countSet++; sumScore += val;
    std::cout << val << '\n';
}

int BotBrain::getHeuristicScore(uint16_t shape, int X, int Y) {
    uint16_t state = EMPTYLINE;

    int val[numSpecLimmit][numWeight] = {};
    uint8_t stateUnder = 0;

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        int limitPos = i/(Common::HEIGHT_MAP/numSpecLimmit);
        uint16_t line = (map[i] & FULLMASK(REALWIDTH));
        if (i >= Y && i < Y + BLOCK_EDGE) {
            line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
            stateUnder |= getLine(shape, i - Y);
        }
        line &= FULLMASK(REALWIDTH);


        for (int j = NUMOFFSET; j <= OFFRIGHT; j++)
            if (getBit(state, j) && !getBit(state, j - 1)) {
                val[limitPos][7]++;
            }

        val[limitPos][2] += nBit1(getStateLine((~state) & line & (((~state) & line) << 1)));
        
        if (val[limitPos][3] == 0) {
            val[limitPos][8] += nBit1((state >> (NUMOFFSET + X)) & FULLMASK(BLOCK_EDGE));
            // std::cout << X << " " << Y << " " << i << "->" << val[limitPos][8] << "\n";
            // _sleep(1000);
        }
        
        val[limitPos][3] += nBit1(getStateLine(state & (~line)));


        state |= line;

        val[limitPos][0] += nBit1(getStateLine(state));
        val[limitPos][1] += (nBit1(((state | (state << 1)) - (state & ((state << 1) | 1))) & FULLMASK(REALWIDTH)) - 2);
        val[limitPos][4] += (state == line && nBit0(getStateLine(state)) == 1);
        val[limitPos][5] += (state == line && nBit0(getStateLine(state)) == 2 && nBit0(getStateLine((state << 1) | state)) == 1);
        
        
        val[0][6] += (line == FULLMASK(REALWIDTH));
        if (val[0][6] > 1) val[val[0][6] - 1][6]++;
        // if (i >= 20) {
        //     err << "iTH LINE: " << i << "limit pos: " << limitPos << '\n';
        //     err << "Line : "; for (int i = 0; i < REALWIDTH; i++) err << getBit(line, i); err << '\n';
        //     err << "State: "; for (int i = 0; i < REALWIDTH; i++) err << getBit(state, i); err << '\n';
    
        //     for (int i = 0; i < numWeight; i++) 
        //         err << "VAL[" << i << "]: " << val[limitPos][i] << "\n"; 
        //     err << '\n';
        // }
    }

    int heuristicScore = 0;
    for (int i = 0; i < numSpecLimmit; i++) 
        for (int j = 0; j < numWeight; j++) {
            heuristicScore += weight[i][j] * val[i][j];
        }

    return heuristicScore;
}


// int BotBrain::getHeuristicScore(uint16_t shape, int X, int Y) {
//     uint16_t state = EMPTYLINE;
//     const int NWEIGHT = 6;

//     int weight[NWEIGHT] = {-300, -7000, -100, -75, 300, 1000};
//     int val[NWEIGHT] = {0};

//     for (int i = 0; i < Common::HEIGHT_MAP; i++) {
//         uint16_t line = (map[i] & FULLMASK(REALWIDTH));
//         if (i >= Y && i < Y + BLOCK_EDGE) line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
//         line &= FULLMASK(REALWIDTH);

//         for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
//             if (getBit(state, j) && !getBit(state, j - 1)) {
//                 val[0]++;
//             }
//             if (getBit(state, j) && !getBit(line, j))
//                 val[1]++;
//         }

//         state |= line;

//         val[2] += __builtin_popcount((state >> NUMOFFSET) & FULLMASK(10));
//         // val[3] += __builtin_popcount((((state | (state >> 1)) ^ (state & (state >> 1))) & FULLMASK(OFFRIGHT - 1)) >> NUMOFFSET);
//         // val[3] += __builtin_popcount((((state | (state << 1)) ^ (state & (state << 1))) & FULLMASK(OFFRIGHT)) >> (NUMOFFSET + 1));
//         val[3] += __builtin_popcount(((((state >> NUMOFFSET) & FULLMASK(10)) | ((state >> NUMOFFSET) & FULLMASK(10)>>1)) ^ ((state >> NUMOFFSET) & FULLMASK(10) & ((state >> NUMOFFSET) & FULLMASK(10)>>1))));
//         val[4] += (__builtin_popcount((state & FULLMASK(REALWIDTH))) == REALWIDTH - 1);
//         val[5] += (line == FULLMASK(REALWIDTH));
//     }

//     int heuristicScore = 0;
//     for (int i = 0; i < NWEIGHT; i++) {
//         heuristicScore += weight[i] * val[i];
//     }

//     return heuristicScore;
// }