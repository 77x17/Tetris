#ifndef CURRENTBLOCK_HPP
#define CURRENTBLOCK_HPP

#include <ncurses.h>
#include <cstdint>

#include <ncurses.h>
#include "Common.hpp"
#include "Block.hpp"

class CurrentBlock {
private:
    Block* block;
    int8_t posX, posY;

public:
    CurrentBlock(Block *a);

    void moveDown();
    void moveLeft();
    void moveRight();

    void draw(WINDOW* win, char c);
};

#endif