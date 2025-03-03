#ifndef MAP_H
#define MAP_H

#include <ncurses.h>
#include <cstdint>

#include "Common.hpp"
#include "CurrentBlock.hpp"

class Map {
private:
    uint16_t map[HEIGHT];
    WINDOW* win;
public:
    Map(WINDOW*);
    void remove(uint8_t nLines);
    bool add(uint8_t nLines);

    void display();
    void updateCur(CurrentBlock* cur);

    bool isValid();
};

#endif