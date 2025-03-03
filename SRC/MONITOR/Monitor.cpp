#include "Monitor.hpp"

Monitor::Monitor(){}

Monitor::Monitor(WINDOW* win) {
    screenPlayer = new Screen(win);
    nLines = nPieces = 0;
    hold = nullptr;
    *cur = CurrentPiece(*setPiece.get(0));
    next = setPiece.get(1);
    map = new Map();
}

Monitor::~Monitor(){
    delete map;
    delete screenPlayer;
}

bool Monitor::moveProcessing() {
    nodelay(stdscr, TRUE);
    switch (getch()) {
        case ERR:
            return false;
            break;
        case 'a':
            return cur->moveLeft(map);
        case 'd':
            return cur->moveRight(map);
        case 's':
            return cur->moveDown(map);
        default:
            return false;
    }
}