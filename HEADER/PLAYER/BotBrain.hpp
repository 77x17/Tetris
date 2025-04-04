#ifndef BOTBRAIN_HPP
#define BOTBRAIN_HPP

#include <cstdint>
#include <fstream>

class BotBrain {
private:
    int numWeight, numSpecLimmit;
    int64_t **weight, *preWeight, *delta;

    static int8_t countSet;
    static int64_t sumScore;
    int64_t preSumScore;
    
    uint64_t* map; // share pointer!
    std::ofstream err;

public:
    BotBrain(uint64_t* map);
    ~BotBrain();

    void updateWeight();
    void load();

    static void setScore(int val);
    int getScore();
    int getHeuristicScore(uint16_t shape, int X, int Y);
};

#endif