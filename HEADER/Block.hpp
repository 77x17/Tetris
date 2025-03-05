#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <ncurses.h>
#include <cstdint>

class Block {
protected:
    static uint8_t speed;
    uint16_t *state;
    uint8_t stateID;
    char symbol;

public:
    Block();
    ~Block();

    void draw(WINDOW* win, uint8_t y, uint8_t x, uint8_t isOn);
    uint16_t getShape();
    uint16_t getRotateLeft();
    uint16_t getRotateRight();

    void rotateLeft();
    void rotateRight();
};

class IBlock:public Block {
public:
    IBlock();
};

class LBlock:public Block {
public:
    LBlock();
};


class PBlock:public Block {
public:
    PBlock();
};


class OBlock:public Block {
public:
    OBlock();
};


class ZBlock:public Block {
public:
    ZBlock();
};


class SBlock:public Block {
public:
    SBlock();
};

class TBlock:public Block {
public:
    TBlock();
};

#endif