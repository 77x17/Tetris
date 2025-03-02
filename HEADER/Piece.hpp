#ifndef PIECE_H
#define PIECE_H

#include <cstdint>
#include <ncurses.h>

#include "Map.hpp"
#include "Common.hpp"

class Piece {
private:
    uint8_t posX, posY;
    static uint8_t speed;
    uint16_t shape;
    char symbol;
public:
    Piece(uint16_t shape);
    Piece& operator=(const Piece &a);
    virtual ~Piece();
    bool moveDown(Map* map);
    bool moveLeft(Map* map);
    bool moveRight(Map* map);
};

#endif