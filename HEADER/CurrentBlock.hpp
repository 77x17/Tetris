#ifndef CURRENTBLOCK_HPP
#define CURRENTBLOCK_HPP

#include <ncurses.h>
#include <cstdint>

class Block;
class Map;


class CurrentBlock {
private:
    Block* block;
    int8_t posX, posY;

public:
    CurrentBlock(Block *p);
    ~CurrentBlock();

    bool moveDown(Map *map);
    bool moveLeft(Map *map);
    bool moveRight(Map* map);

    uint8_t put(Map* map);
    void draw(Map* map);
    bool isValid(Map* map);
};

#endif