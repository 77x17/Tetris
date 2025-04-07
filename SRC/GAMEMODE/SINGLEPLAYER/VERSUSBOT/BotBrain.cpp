#include "BotBrain.hpp"

#include <iostream>
#include <random>
#include <algorithm>

#include "Common.hpp"
#include "CommonMap.hpp"

#define getStateLine(x) (((x) >> NUMOFFSET) & FULLMASK(Common::WIDTH_MAP))
#define nBit1(x) (__builtin_popcount(x))
#define nBit0(x) (nBit1((~(x)) & FULLMASK(Common::WIDTH_MAP)))

int BotBrain::random(int L, int R) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(L, R);
    return dist(gen); 
}

BotBrain::BotBrain(uint64_t* map) : map(map) {
    numWeight = 10; numSpecLimmit = 4; mutationRateInver = 1;
    weight = new int64_t* [numSpecLimmit]();
    for (int i = 0; i < numSpecLimmit; i++)
        weight[i] = new int64_t[numWeight]();
    load();
    // sizeOfPupulation = 40;
    // generatePopulation();
}

void BotBrain::generatePopulation() {
    population.clear();
    for (int i = 0; i < sizeOfPupulation; i++)
        population.push_back(generateIndividual());
    score.clear(); individual = population[score.size()]; 

    std::fstream f("./err.txt", std::ios_base::app);
    f << "START POPULATION\n";
    for (auto x: population) {
        for (auto y: x)
            f << y << " "; 
        f <<  '\n';
    }
    f << "END\n";
    f.close();
}

vector<int> BotBrain::generateIndividual() {
    vector<int> ans;
    for (int i = 0; i < numWeight; i++)
        ans.push_back(weight[0][i] + random(-5, 5));
    return ans;
}

void BotBrain::load() {
    std::fstream f("./ASSETS/BotBrain.txt");
    
    for (int i = 0; i < numSpecLimmit; i++)
        for (int j = 0; j < numWeight; j++)
            f >> weight[i][j];

    f.close();
}

void BotBrain::writeDown(vector<vector<int>> &a) {
    std::fstream f("./ASSETS/BotBrainNew.txt", std::ios_base::app);
    f << '\n';
    for (int j = 0; j < numWeight; j++)
        f << population[0][j] << " ";
    f << "SCORE: " << population[0].back();
    f.close();
}

BotBrain::~BotBrain() {
    for (int i = 0; i < numSpecLimmit; i++)
        delete[] weight[i];
    delete[] weight;
}

uint8_t BotBrain::getHeightMap() {
    uint8_t ans = 0;
    while (map[ans] == EMPTYLINE) ans++;
    return ans;
}

vector<int> BotBrain::crossover(vector<int> &a, vector<int> &b) {
    vector<int> ans;
    for (int i = 0; i < a.size(); i++)
        if (random(0, 1)) ans.push_back(a[i]);
        else ans.push_back(b[i]);
    return ans;
}

void BotBrain::mutate(vector<int> &a) {
    for (auto &x: a)
        if (random(1, 10) < mutationRateInver)
            x += random(-1, 1);
}

void BotBrain::updateWeight() {
    load(); std::cout << "Succesfully load!\n";
    return;
    std::cout << "SCORE: " << sumScore << '\n';
    for (auto x: individual) std::cout << x << " "; std::cout << '\n';
    if (countSet < 5) return;

    std::fstream f("./err.txt", std::ios_base::app);
    f << "SCORE: " << sumScore << '\n';
    for (auto x: individual) f << x << " "; f<< '\n';
    f.close();

    individual = population[score.size()];
    score.push_back(sumScore);
    sumScore = 0; countSet = 0;

    if (score.size() < sizeOfPupulation) return;

    for (int i = 0; i < sizeOfPupulation; i++) 
        population[i].push_back(score[i]);

    sort(population.begin(), population.end(), [&](vector<int> &a, vector<int> &b) {
        return a.back() > b.back();
    });

    writeDown(population);

    for (auto &x: population) x.pop_back();

    vector<vector<int>> newPopulation;
    while (newPopulation.size() < sizeOfPupulation) {
        vector<int> child1 = population[random(0, 9)];
        vector<int> child2 = population[random(0, 9)];
        vector<int> child = crossover(child1, child2); mutate(child);
        newPopulation.push_back(child);
    }

    std::cout << "Max Score: " << population.front().back() << '\n';

    population = newPopulation;
    score.clear();
}

void BotBrain::setScore(int val) {
    countSet++; sumScore += val;
    std::cout << val << '\n';
}


// int BotBrain::getHeuristicScore(uint16_t shape, int X, int Y) {
    
//     uint16_t state = EMPTYLINE, stateAND = EMPTYLINE, endStateHole = EMPTYLINE;

//     for (int i = 0; i < Common::HEIGHT_MAP; i++) {
//         uint16_t line = (map[i] & FULLMASK(REALWIDTH));
//         if (i >= Y && i < Y + BLOCK_EDGE) {
//             line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
//         }
//         line &= FULLMASK(REALWIDTH);
//         for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
//             if (!getBit(state, j) && getBit(line, j)) {
//                 endStateHole |= MASK(j);
//             }
//         }
//         endStateHole &= line;
//     }
//     endStateHole = ((~endStateHole) & FULLMASK(REALWIDTH));

//     int val[numWeight] = {};
//     int top[10] = {};  for (int i = 0; i < 10; i++) top[i] = 24;

//     for (int i = 0; i < Common::HEIGHT_MAP; i++) {
//         uint16_t line = (map[i] & FULLMASK(REALWIDTH));
//         if (i >= Y && i < Y + BLOCK_EDGE) {
//             line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
//         }
//         line &= FULLMASK(REALWIDTH);


//         for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
//             if (getBit(state, j) && !getBit(state, j - 1)) {
//                 val[7]++;
//             }
//             if (!getBit(state, j) && getBit(line, j)) {
//                 top[j - NUMOFFSET] = i;
//                 stateAND |= MASK(j);
//             }
//         }

//         val[2] += nBit1(getStateLine((~state) & line & (((~state) & line) << 1)));
                
//         val[3] += nBit1(getStateLine(state & (~line))) * (Common::HEIGHT_MAP - i);

//         state |= line;
//         stateAND &= line;

//         val[0] += nBit1(getStateLine(state));

//         val[1] += (nBit1(((state | (state << 1)) - (state & ((state << 1) | 1))) & FULLMASK(REALWIDTH)) - 2);
//         val[4] += (state == line && nBit0(getStateLine(state)) == 1);
//         val[5] += (state == line && nBit0(getStateLine(state)) == 2 && nBit0(getStateLine((state << 1) | state)) == 1);
        
//         val[6] += (line == FULLMASK(REALWIDTH));
//         val[8] += nBit1(stateAND & endStateHole);
//     }

//     int min = 300; 
//     for (int i = 0; i < 10; i++) min = std::min(min, top[i]);

//     for (int i = 1; i < 10; i++) {
//         val[9] += (top[i] - min);
//     }

//     int heuristicScore = 0;
//     for (int j = 0; j < numWeight; j++) {
//         heuristicScore += individual[j] * val[j];
//     }

//     return heuristicScore;
// }

int BotBrain::getHeuristicScore(uint16_t shape, int X, int Y) {
    
    const int limitPos = getHeightMap()/(Common::HEIGHT_MAP/numSpecLimmit);

    uint16_t state = EMPTYLINE, stateAND = EMPTYLINE, endStateHole = EMPTYLINE, prevLine = EMPTYLINE;

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        uint16_t line = (map[i] & FULLMASK(REALWIDTH));
        if (i >= Y && i < Y + BLOCK_EDGE) {
            line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
        }
        line &= FULLMASK(REALWIDTH);
        if (line != FULLMASK(REALWIDTH)) {
            for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
                if (!getBit(state, j) && getBit(line, j)) {
                    endStateHole |= MASK(j);
                }
            }
            endStateHole &= line;
        }
    }
    endStateHole = ((~endStateHole) & FULLMASK(REALWIDTH));

    int val[numSpecLimmit][numWeight] = {};
    int top[10] = {};  for (int i = 0; i < 10; i++) top[i] = 24;

    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        uint16_t line = (map[i] & FULLMASK(REALWIDTH));
        if (i >= Y && i < Y + BLOCK_EDGE) {
            line ^= (getLine(shape, i - Y) << (X + NUMOFFSET));
        }
        line &= FULLMASK(REALWIDTH);

        if (line == FULLMASK(REALWIDTH)) {
            val[limitPos][6] += 1;
            // else (val[limitPos][6] > 1) val[limitPos][6] *= 10;
            continue;
        }

        for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
            if (getBit(state, j) && !getBit(state, j - 1)) {
                val[limitPos][7]++;
            }
            if (!getBit(state, j) && getBit(line, j)) {
                top[j - NUMOFFSET] = i - val[limitPos][6];
                stateAND |= MASK(j);
            }
        }
        val[limitPos][2] += nBit1(getStateLine((~state) & line & (((~state) & line) << 1)));
        val[limitPos][3] += nBit1(getStateLine(state & (~line) & prevLine)) * (Common::HEIGHT_MAP - i);

        state |= line; prevLine = line;
        stateAND &= line;

        val[limitPos][0] += nBit1(getStateLine(state));

        val[limitPos][1] += (nBit1(((state | (state << 1)) - (state & ((state << 1) | 1))) & FULLMASK(REALWIDTH)) - 2);
        val[limitPos][4] += (state == line && nBit0(getStateLine(state)) == 1);
        val[limitPos][5] += (state == line && nBit0(getStateLine(state)) == 2 && nBit0(getStateLine((state << 1) | state)) == 1);
        
        if (val[limitPos][3] == 0) val[limitPos][8] += nBit1(stateAND & endStateHole);
    }

    int min = 300; 
    for (int i = 0; i < 10; i++) min = std::min(min, top[i]);

    for (int i = 1; i < 10; i++) {
        val[limitPos][9] += (top[i] - min);
    }
    
    if (val[limitPos][6] == 4) val[limitPos][6] = 1000;

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