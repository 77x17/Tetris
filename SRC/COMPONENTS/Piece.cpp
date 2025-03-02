#include "Piece.hpp"

Piece::Piece(uint16_t _shape) {
    shape = _shape;
    posX = 0; posY = WIDTH / 2 + 2;
}

Piece& Piece::operator=(const Piece &a) {
    if (this == &a) return *this;
    posX = a.posX; posY = a.posY;
    shape = a.shape;
    return *this;
}

Piece::~Piece() {}

bool Piece::moveDown(Map* map) { posY++; }

bool Piece::moveLeft(Map* map) {}

bool Piece::moveRight(Map* map) {}