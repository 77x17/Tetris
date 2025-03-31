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
    
    sf::Texture texture;
    
    void remove(uint8_t pos);

protected:
    uint64_t* map;
    
public:
    Map();
    virtual ~Map();
    void setPosition(int GRID_POSITION_X, int GRID_POSITION_Y, int GRID_WIDTH, int GRID_HEIGHT);
    
    void reset();

    void draw(sf::RenderWindow* window);
    void drawCurrentBlock(sf::RenderWindow* window, CurrentBlock *curBlock);
    void drawOutline(sf::RenderWindow* window);

    virtual uint8_t putBlockIntoMap(Block* block, int Y, int X);
    bool isValid(uint16_t shape, int Y, int X);
    bool isAllClear();
};

#endif