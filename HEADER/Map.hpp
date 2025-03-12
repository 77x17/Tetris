#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <SFML/Graphics.hpp>

#include "Common.hpp"

#define HEIGHT 24
#define WIDTH  10

class Block;
class CurrentBlock;

class Map {
private:
    int GRID_POSITION_X;
    int GRID_POSITION_Y;
    int GRID_WIDTH;
    int GRID_HEIGHT;

    uint64_t map[HEIGHT + 1];

    sf::Texture texture;

    void remove(uint8_t pos);

public:
    Map(int GRID_POSITION_X, int GRID_POSITION_Y, int GRID_WIDTH, int GRID_HEIGHT);
    ~Map();
    
    bool add(uint8_t nLines);

    void draw(sf::RenderWindow* window);
    void drawCurrentBlock(sf::RenderWindow* window, Block *block, int posY, int shadowPosY, int posX);
    void drawOutline(sf::RenderWindow* window);

    uint8_t update(Block* block, int Y, int X);
    bool isValid(uint16_t shape, int Y, int X);
};

#endif