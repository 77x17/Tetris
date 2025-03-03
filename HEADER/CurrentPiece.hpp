#ifndef CURRENTPIECE_HPP
#define CURRENTPIECE_HPP

#include <ncurses.h>
#include <cstdint>

#include "Common.hpp"
#include "Piece.hpp"
#include "Screen.hpp"

class CurrentPiece: public Piece {
private:
    int8_t posX, posY;
public:
    CurrentPiece(const Piece&a);

    bool moveDown(Map* map);
    bool moveLeft(Map* map);
    bool moveRight(Map* map);

    void draw(Screen* screen, char c);
};

    
#endif