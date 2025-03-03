#include "Monitor.hpp"

Monitor::Monitor(WINDOW* _win) {
    win = _win;
    hold = new Hold(win);
    next = new LinkListBlock(win);
    next->updateNext(curBlock);
    map = new Map(win);
    infor = new Infor(win);
}

Monitor::~Monitor(){
    delete curBlock;
    delete hold;
    delete next;
    delete map;
    delete infor;
}

bool Monitor::moveProcessing() {
    nodelay(stdscr, TRUE);
    switch (getch()) {
        case ERR:
            return false;
            break;
        case 'a':
            curBlock->moveDown();
            break;
        case 'd':
            curBlock->moveRight();
            break;
        case 's':
            curBlock->moveDown();
            break;
        default:
            return false;
    }
    return true;
}