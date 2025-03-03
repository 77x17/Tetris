#include "Screen.hpp"

Screen::Screen(){}

Screen::Screen(WINDOW *win) {
    winMap = derwin(win, 22, 12, 2, 10);
    winInfor = winHold = winNext = winSpec = nullptr;
    box(winMap, 0, 0);
    refresh();
}

Screen::~Screen() {
    delwin(winMap);
}

void Screen::drawHold(Piece* piece) {
}

void Screen::drawPiece(uint16_t shape, char c, uint8_t posY, uint8_t posX) {
    for (int i = 0; i < SIZEPIECE; i++)
        if (getBit(shape, i))
            mvwaddch(winMap, posY + i / PIECE_EDGE, posX + i % PIECE_EDGE, c);
    wrefresh(winMap);
}

// void Screen::showInfor() {

// }

void Screen::drawMap(Map *map) {
    map->display(winMap);
}

// void Screen::drawNext() {

// }
