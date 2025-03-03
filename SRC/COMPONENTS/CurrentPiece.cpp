#include "CurrentPiece.hpp"

CurrentPiece::CurrentPiece(const Piece& a) : Piece(a), posX(WIDTH / 2 - PIECE_EDGE/2), posY(0){}

bool CurrentPiece::moveDown(Map* map) {
    posY++;
    return true;
}

bool CurrentPiece::moveLeft(Map* map) {
    posX--;
    return true;
}

bool CurrentPiece::moveRight(Map* map) {
    posX++;
    return true;
}

void CurrentPiece::draw(Screen* screen, char c) {
    Piece::draw(screen, c, posY, posX);
}