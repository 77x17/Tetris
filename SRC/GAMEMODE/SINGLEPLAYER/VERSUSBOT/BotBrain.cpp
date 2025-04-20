#include "BotBrain.hpp"

#include <iostream>
#include <random>
#include <algorithm>

#include "CommonMap.hpp"

#define getStateLine(x) (((x) >> NUMOFFSET) & FULLMASK(WIDTH_MAP))
#define nBit1(x) (__builtin_popcount(x))
#define nBit0(x) (nBit1((~(x)) & FULLMASK(WIDTH_MAP)))

int BotBrain::random(int L, int R) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(L, R);
    return dist(gen); 
}

BotBrain::BotBrain(uint64_t* map) : map(map) {
    mutationRateInver = 1;
    numWeight = 10; numSpecLimmit = 4;

    weight = new int64_t* [numSpecLimmit]();
    for (int i = 0; i < numSpecLimmit; i++)
        weight[i] = new int64_t[numWeight]();
    shapeWeight = new int64_t[7]();
    lineLimit = new uint8_t[numSpecLimmit]();

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
     
    for (int i = 0; i < numSpecLimmit; i++) {
        uint32_t t = 0; f >> t;
        lineLimit[i] = t; // đọc theo dạng kí tự do uint8_t define của char!!!!
    }

    for (int i = 0; i < numSpecLimmit; i++)
        for (int j = 0; j < numWeight; j++)
            f >> weight[i][j];

    for (int i = 0; i < 7; i++)
        f >> shapeWeight[i];

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
    delete[] shapeWeight;
    delete[] lineLimit;
}

uint8_t BotBrain::getLineLimit(uint8_t Y) {
    uint8_t h = 0;
    while (map[h] == EMPTYLINE) h++;
    if (h > Y) h = Y;
    uint8_t ans = 0; while (ans < numSpecLimmit &&  lineLimit[ans] <= h) ans++;
    if (ans == numSpecLimmit) {
        std::cout << (int)ans << " " << (int)h << " " << (int)lineLimit[numSpecLimmit - 1] << '\n';
        throw std::out_of_range("ans is larger than numSpecLimit! " + std::to_string(ans));
    }
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
    // load(); std::cout << "Succesfully load!\n";
    return;
    std::cout << "SCORE: " << sumScore << '\n';
    for (auto x: individual) std::cout << x << " "; std::cout << '\n';
    if (countSet < 5) return;

    std::fstream f("./err.txt", std::ios_base::app);
    f << "SCORE: " << sumScore << '\n';
    for (auto x: individual) f << x << " "; f << '\n';
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

int BotBrain::getHeuristicScore(uint16_t shape, uint8_t shapeID, int X, int Y) {

    const int limitPos = getLineLimit();

    uint16_t state = EMPTYLINE, stateAND = EMPTYLINE, endStateHole = EMPTYLINE, prevLine = EMPTYLINE;

    for (int i = 0; i < HEIGHT_MAP; i++) {
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
    int top[10] = {};  for (int i = 0; i < 10; i++) top[i] = HEIGHT_MAP;

    for (int i = 0; i < HEIGHT_MAP; i++) {
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
        int cnt = 0;
        for (int j = NUMOFFSET; j <= OFFRIGHT; j++) {
            if (getBit(state, j) && !getBit(state, j - 1)) {
                val[limitPos][7]++; cnt++;
            }
            if (!getBit(state, j) && getBit(line, j)) {
                top[j - NUMOFFSET] = i - val[limitPos][6];
                stateAND |= MASK(j);
            }
        }
        val[limitPos][2] += nBit1(getStateLine((~state) & line & (((~state) & line) << 1)));
        val[limitPos][3] += nBit1(getStateLine(state & (~line) & prevLine)) * (HEIGHT_MAP - i);

        state |= line;
        stateAND &= line;

        val[limitPos][0] += nBit1(getStateLine(state));

        val[limitPos][1] += (nBit1(((state | (state << 1)) - (state & ((state << 1) | 1))) & FULLMASK(REALWIDTH)) - 2);
        val[limitPos][4] += (nBit0(getStateLine(state)) == 1);
        if (nBit0(getStateLine(state)) == 1 && state != line && line != prevLine)
            val[limitPos][4] >>= 1;

        if (cnt == 1 && state == line && nBit0(getStateLine(state)) > 1) val[limitPos][5]++;
        
        if (val[limitPos][3] == 0) val[limitPos][8] += nBit1(stateAND & endStateHole);
        
        prevLine = line;
    }

    int min = HEIGHT_MAP;
    for (int i = 0; i < 10; i++) min = std::min(min, top[i]);

    for (int i = 1; i < 10; i++) {
        val[getLineLimit(Y)][9] += (top[i] - min);
    }

    if (val[limitPos][6] == 4) val[limitPos][6] = 5000;
    
    int heuristicScore = shapeWeight[shapeID - 1];
    for (int i = 0; i < numSpecLimmit; i++) 
        for (int j = 0; j < numWeight; j++) {
            heuristicScore += weight[i][j] * val[i][j];
        }

    return heuristicScore;
}
