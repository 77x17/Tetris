#ifndef PLAYERSCREEN_HPP
#define PLAYERSCREEN_HPP

#include <cstdint>
#include <ncurses.h>
#include "Piece.hpp"
#include "SetPiece.hpp"
#include "Map.hpp"

class Screen {
private:
    WINDOW *winHold, *winInfor, *winMap, *winNext, *winSpec;
public:
    Screen();
    Screen(WINDOW *win);
    ~Screen();
    void drawHold(Piece* piece);
    // void showInfor();
    void drawMap(Map* map);
    // void drawNext(PieceInSet* piece, int8_t number = 4);
};

#endif