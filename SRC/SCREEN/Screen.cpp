#include "Screen.hpp"

Screen::Screen(){}

Screen::Screen(WINDOW *win) {
    box(winMap, 0, 0);
    winMap = subwin(win, 22, 12, 10, 10);
    winInfor = winHold = winNext = winSpec = nullptr;
    refresh();
}

Screen::~Screen() {
    delwin(winMap);
}

void Screen::drawHold(Piece* piece) {

}

// void Screen::showInfor() {

// }

void Screen::drawMap(Map *map) {
    map->display(winMap);
}

// void Screen::drawNext() {

// }
