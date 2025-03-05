#ifndef CURRENTBLOCK_HPP
#define CURRENTBLOCK_HPP

#include <ncurses.h>
#include <cstdint>

class Block;
class Map;
class Hold;


class CurrentBlock {
private:
    Block* block;
    int8_t posX, posY;

public:
    CurrentBlock(Block *p);
    ~CurrentBlock();

    bool isEmpty();
    void setter(Block* p);

    bool moveDown(Map *map);
    bool moveLeft(Map *map);
    bool moveRight(Map* map);

    bool rotateLeft(Map* map);
    bool rotateRight(Map* map);

    bool swap(Hold* hold);

    uint8_t put(Map* map);
    void draw(Map* map, bool isOn);
    bool isValid(uint16_t shape, Map* map);
};

#endif