#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <SFML/Graphics.hpp>

#include "Monitor.hpp"

class Monitor;
class Block;
class CurrentBlock;

class Map {
private:
    int GRID_POSITION_X;
    int GRID_POSITION_Y;
    int GRID_WIDTH;
    int GRID_HEIGHT;

    sf::Texture texture;

protected:
    uint64_t* map;

    void remove(uint8_t pos);
    void setPosition(int GRID_POSITION_X, int GRID_POSITION_Y, int GRID_WIDTH, int GRID_HEIGHT);

public:
    Map();
    ~Map();
    
    void reset();

    bool addPosible(uint64_t nLiness);

    void draw(sf::RenderWindow* window);
    void drawCurrentBlock(sf::RenderWindow* window, Block *block, int posY, int shadowPosY, int posX);
    void drawOutline(sf::RenderWindow* window);

    uint8_t putBlockIntoMap(Block* block, int Y, int X);
    bool isValid(uint16_t shape, int Y, int X);

    friend void Monitor::setPosition(int X_COORDINATE, int Y_COORDINATE);
};

#endif