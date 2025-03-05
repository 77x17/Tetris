#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

Monitor::Monitor(WINDOW* _win) {
    win = _win;
    map = new Map(win);
    hold = new Hold(win);
    next = new LinkListBlock(win);
    curBlock = next->updateNext();
    infor = new Infor(win);
}

Monitor::~Monitor(){
    if (curBlock) {delete curBlock; curBlock = nullptr;}
    if (hold) {delete hold; hold = nullptr;}
    if (next) {delete next; next = nullptr;}
    if (map) {delete map; map = nullptr;}
    if (infor) {delete infor; infor = nullptr;}
}

bool Monitor::moveProcessing() {
    nodelay(stdscr, TRUE);

    switch (getch()) {
        case ERR:
            return false;
            break;
        case 'a':
        case KEY_LEFT:
            return curBlock->moveLeft(map);
        case 'd':
        case KEY_RIGHT:
            return curBlock->moveRight(map);
        case 's':
        case KEY_DOWN:
            return curBlock->moveDown(map);
        case 'w':
        case KEY_UP:
            return curBlock->rotateLeft(map);
        default:
            return false;
    }
    return true;
}
