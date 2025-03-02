#ifndef MONITOR_HPP
#define MONITOR_HPP

#include "Piece.hpp"
#include "SetPiece.hpp"
#include "Screen.hpp"

class Monitor {
private:
    int8_t nLines, nPieces;
    Piece *hold, *cur, *next;
    Map* map;
    static SetPiece setPiece;
    Screen screenPlayer;
public:
    Monitor();
    Monitor(WINDOW* win);
    ~Monitor();
    bool moveProcessing();
    virtual void start()=0;
};

#endif