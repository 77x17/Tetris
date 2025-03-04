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
    delete curBlock;
    delete hold;
    delete next;
    delete map;
    delete infor;
}

bool Monitor::moveProcessing() {
    nodelay(stdscr, TRUE);
    bool flag = false;

    switch (getch()) {
        case ERR:
            return false;
            break;
        case 'a':
            return curBlock->moveLeft(map);
        case 'd':
            return curBlock->moveRight(map);
        case 's':
            return curBlock->moveDown(map);
        default:
            return false;
    }
    return true;
}