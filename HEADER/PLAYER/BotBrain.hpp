#ifndef BOTBRAIN_HPP
#define BOTBRAIN_HPP

#include <cstdint>

class BotBrain {
private:
    int* weight;
    int preScore;
    uint64_t* map; // share pointer!

public:
    BotBrain(uint64_t* map);
    ~BotBrain();
    int getScore();
    int getHeuristicScore(uint16_t shape, int X, int Y);
};

#endif