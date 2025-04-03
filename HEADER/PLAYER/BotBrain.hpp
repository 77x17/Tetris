#ifndef BOTBRAIN_HPP
#define BOTBRAIN_HPP

#include <cstdint>

class BotBrain {
private:
    int numWeight;
    int* weight, *preWeight;

    static int8_t countSet;
    static uint64_t sumScore;
    uint64_t preSumScore;
    
    uint64_t* map; // share pointer!

public:
    BotBrain(uint64_t* map);
    ~BotBrain();
    static void setScore(uint64_t val);
    int getScore();
    int getHeuristicScore(uint16_t shape, int X, int Y);
};

#endif