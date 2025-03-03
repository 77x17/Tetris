#ifndef PIECE_H
#define PIECE_H

#include <cstdint>
#include <ncurses.h>

#include "Map.hpp"
#include "Common.hpp"
#include "Screen.hpp"

class Piece {
protected:
    uint16_t shape;
    char symbol;
    static uint8_t speed;

    uint8_t cnt;
    Piece* nextPiece;
public:
    Piece(int numPlayer, uint16_t shape);
    Piece(const Piece &a);
    ~Piece();
    Piece& operator=(const Piece &a);
    void draw(Screen *screen, char ch, uint8_t posY, uint8_t posX);

    friend class SetPiece;
    friend class CurrentPiece;
};

#endif