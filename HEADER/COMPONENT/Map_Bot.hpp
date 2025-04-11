#ifndef MAP_BOT_HPP
#define MAP_BOT_HPP

#include "Map_Multiplayer.hpp"

class CurrentBlock_Bot;
class BotBrain;

class Map_Bot: public Map_Multiplayer {
private:
    BotBrain* botBrain;
public:
    Map_Bot();
    ~Map_Bot();

    void reset();

    int getHeuristicScore(uint16_t shape, int X, int Y);
    void findPath(int8_t &X, int8_t &timeRotate, bool &isHold, CurrentBlock_Bot* CurrentBlock, CurrentBlock_Bot* tmp);
};

#endif