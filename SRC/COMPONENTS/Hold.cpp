#include "Hold.hpp"

Hold::Hold(WINDOW* _win) {
    win = derwin(_win, 6, 6, 1, 2);
    box(win, 0, 0);
    mvwaddstr(win, 0, 1, "HOLD");
    wrefresh(win);
}

Hold::~Hold() { delwin(win); }

void Hold::draw() {
    block->draw(win, 1, 1);
}