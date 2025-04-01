#ifndef MAP_BOT_HPP
#define MAP_BOT_HPP

#include "Map_Multiplayer.hpp"

class CurrentBlock;

class Map_Bot: public Map_Multiplayer {
public:
    Map_Bot();
    ~Map_Bot();

    int getScore(CurrentBlock*);
};

#endif