#include "Hold.hpp"

#include "Block.hpp"

Hold::Hold(sf::RenderWindow* newWindow) {
    block = nullptr;
    window = newWindow;
    // box(win, 0, 0);
    // mvwaddstr(win, 0, 1, "HOLD");
    // wrefresh(win);
}

Hold::~Hold() { 
    if (block) delete block; 
    block = nullptr;
}

Block* Hold::interchange(Block* p) {
    Block* tmp = block; block = p;
    return tmp;
}

bool Hold::canHold() {
    return holdPosible;
}

void Hold::lock() { holdPosible = false; }
void Hold::unlock() { holdPosible = true; }

void Hold::draw() {
    if (block) block->draw(window, 1, 1, -1);
}

void Hold::erase() {
    if (block) block->draw(window, 1, 1, ' ');
}