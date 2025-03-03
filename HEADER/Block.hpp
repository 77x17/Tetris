#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <ncurses.h>

#include "Common.hpp"

class Block {
protected:
    static uint8_t speed;
    uint16_t *shape;
    uint8_t cur;

    virtual uint16_t getShape() = 0;
    virtual char getSymbol() = 0;

public:
    void draw(WINDOW* win, uint8_t y, uint8_t x, uint8_t type=0);
    void rotateLeft();
    void rotateRight();
};

class IBlock:public Block {
private:
    char symbol;
    uint16_t getShape();
    char getSymbol();
public:
    IBlock();
    ~IBlock();
};

#endif