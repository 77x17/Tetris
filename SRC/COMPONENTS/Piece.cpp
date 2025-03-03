#include "Piece.hpp"

Piece::Piece(int numPlayer, uint16_t _shape) {
    cnt = numPlayer;
    shape = _shape;

    nextPiece = nullptr;
    // posX = WIDTH / 2 - 2;
    // posY = 1; 
}

Piece::Piece(const Piece &a) {
    shape = a.shape; symbol = a.symbol;
    speed = a.speed; cnt = a.cnt;
    nextPiece = a.nextPiece;
}

Piece& Piece::operator=(const Piece &a) {
    if (this == &a) return *this;
    shape = a.shape;
    return *this;
}

Piece::~Piece() {

}

void Piece::draw(Screen* screen, char c, uint8_t posY, uint8_t posX) {
    screen->drawPiece(shape, c, posY, posX);
}