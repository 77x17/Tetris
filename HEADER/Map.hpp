#ifndef MAP_H
#define MAP_H

// #include <ncurses.h>
#include <cstdint>
#include <SFML/Graphics.hpp>

#include "Common.hpp"

class Block;
class CurrentBlock;

class Map {
private:
    uint16_t map[HEIGHT + 1];
    sf::RenderWindow* window;

    void remove(uint8_t pos);
public:
    Map(sf::RenderWindow* newWindow);
    ~Map();
    
    bool add(uint8_t nLines);

    void draw();
    void drawOutline(sf::RenderWindow* window);
    void drawCur(Block* block, int Y, int X);
    void eraseCur(Block* block, int Y, int X);

    uint8_t update(uint16_t shape, int Y, int X);
    bool isValid(uint16_t shape, int Y, int X);

};

#endif