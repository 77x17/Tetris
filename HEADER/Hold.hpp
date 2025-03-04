#ifndef HOLD_HPP
#define HOLD_HPP

#include <ncurses.h>

class Block; 

class Hold {
private:
    Block* block;
    WINDOW* win;
public:
    Hold(WINDOW* win);
    ~Hold();
    void draw();
};

#endif