#include "Hold.hpp"

#include "Common.hpp"
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

void Hold::drawOutline(sf::RenderWindow* window) {
    sf::RectangleShape line(sf::Vector2f(HOLD_WIDTH * BLOCK_SIZE, 1));
    line.setFillColor(sf::Color(200, 200, 200, 150)); // Gray
    line.setPosition(HOLD_POSITION_X, HOLD_POSITION_Y + 0           * BLOCK_SIZE);
    window->draw(line);
    line.setPosition(HOLD_POSITION_X, HOLD_POSITION_Y + HOLD_HEIGHT * BLOCK_SIZE);
    window->draw(line);
    
    line.setSize(sf::Vector2f(1, HOLD_HEIGHT * BLOCK_SIZE));
    line.setPosition(HOLD_POSITION_X + 0          * BLOCK_SIZE, HOLD_POSITION_Y);
    window->draw(line);
    line.setPosition(HOLD_POSITION_X + HOLD_WIDTH * BLOCK_SIZE, HOLD_POSITION_Y);
    window->draw(line);
}

void Hold::draw() {
    if (block) block->draw(window, 1, 1);
}

void Hold::erase() {
    if (block) block->draw(window, 1, 1);
}