#ifndef HOLD_HPP
#define HOLD_HPP

#include <ncurses.h>

class Block; 

class Hold {
private:
    Block* block;
    bool holdPosible;
    WINDOW* win;
public:
    Hold(WINDOW* win);
    ~Hold();
    Block* interchange(Block* p);
    bool canHold();
    void lock();
    void unlock();
    void draw();
};

#endif