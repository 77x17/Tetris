#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <ncurses.h>

#include "Common.hpp"
#include "Piece.hpp"

class SetPiece {
private:
    int nPlayer;
    Piece* head;
    Piece* tail;

    Piece* getNewNode(int typeRandom);

public:
    SetPiece(int nPlayer);
    ~SetPiece();
    void updateNext();
    Piece* get(int num);
};

#endif