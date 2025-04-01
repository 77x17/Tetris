#ifndef MAP_BOT_HPP
#define MAP_BOT_HPP

#include "Map_Multiplayer.hpp"

class CurrentBlock_Bot;

class Map_Bot: public Map_Multiplayer {
public:
    Map_Bot();
    ~Map_Bot();

    int getScore(uint16_t shape, int X, int Y);
    void findPath(int8_t &X, CurrentBlock_Bot* curBlock);
};

#endif