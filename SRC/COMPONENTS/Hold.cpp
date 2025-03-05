#include "Hold.hpp"

#include "Block.hpp"

Hold::Hold(WINDOW* _win) {
    block = nullptr;
    win = derwin(_win, 6, 6, 2, 2);
    box(win, 0, 0);
    mvwaddstr(win, 0, 1, "HOLD");
    wrefresh(win);
}

Hold::~Hold() { 
    if (block) delete block; 
    block = nullptr;
    delwin(win); 
}

void Hold::draw() {
    block->draw(win, 1, 1, 1);
}