#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include <cstdint>

#include "Common.hpp"

class Map {
private:
    uint16_t map[HEIGHT];
public:
    Map();
    void remove(uint8_t nLines);
    bool add(uint8_t nLines);
    void display(WINDOW* win);
    // bool isValid(Block *p);
};

#endif