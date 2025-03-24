#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <SFML/Graphics.hpp>

#include "Common.hpp"

class Block;
class CurrentBlock;

class Map {
private:
    int GRID_POSITION_X;
    int GRID_POSITION_Y;
    int GRID_WIDTH;
    int GRID_HEIGHT;

    uint64_t* map;

    sf::Texture texture;

    void remove(uint8_t pos);

    
public:
    Map();
    ~Map();
    void setPosition(int GRID_POSITION_X, int GRID_POSITION_Y, int GRID_WIDTH, int GRID_HEIGHT);
    
    void reset();

    bool addPosible(uint64_t nLiness);

    void add(uint64_t nLines, int seed);

    void draw(sf::RenderWindow* window);
    void drawCurrentBlock(sf::RenderWindow* window, CurrentBlock *curBlock);
    void drawOutline(sf::RenderWindow* window);

    uint8_t putBlockIntoMap(Block* block, int Y, int X);
    bool isValid(uint16_t shape, int Y, int X);
};

#endif