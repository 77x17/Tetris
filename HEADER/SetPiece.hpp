#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <ncurses.h>

#include "Common.hpp"
#include "Piece.hpp"

class SetInPiece: public Piece {
private:
    uint8_t cnt;
    SetInPiece* nextPiece;
public:
    SetInPiece(int numPlayer, uint16_t shape);
    ~SetInPiece();
    friend class SetPiece;
};

class SetPiece {
private:
    int nPlayer;
    SetInPiece* head;
    SetInPiece* tail;

    SetInPiece* getNewNode(int typeRandom);

public:
    SetPiece(int nPlayer);
    ~SetPiece();
    void updateNext();
    SetInPiece* get(int num);
};

#endif