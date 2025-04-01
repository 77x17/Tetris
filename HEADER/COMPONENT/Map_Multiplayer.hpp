#ifndef MAP_MULTIPLAYER_HPP
#define MAP_MULTIPLAYER_HPP

#include "Map.hpp"

class Map_Multiplayer: public Map {
public:
    Map_Multiplayer();
    virtual ~Map_Multiplayer();

    void drawOutline(sf::RenderWindow* window);

    bool addPosible(uint64_t nLines);
    void add(uint64_t nLines, int seed);

    uint8_t putBlockIntoMap(Block* block, int Y, int X);
};

#endif