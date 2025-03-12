#include "Hold.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Monitor.hpp"

Hold::Hold() {
    block = nullptr;
    holdPosible = true;
}

Hold::~Hold() { 
    if (block) delete block; 
    block = nullptr;
}

Block* Hold::interchange(Block* p) {
    Block* temp = block; block = p;
    block->resetState();
    return temp;
}

bool Hold::canHold() {
    return holdPosible;
}

void Hold::lock() { holdPosible = false; }
void Hold::unlock() { holdPosible = true; }

void Hold::drawOutline(sf::RenderWindow* window) {
    sf::Font font;
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
    sf::Text holdText("HOLD", font, BLOCK_SIZE - BLOCK_SIZE / 4);
    holdText.setPosition(HOLD_POSITION_X, HOLD_POSITION_Y - BLOCK_SIZE);
    window->draw(holdText);

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

void Hold::draw(sf::RenderWindow *window) {
    if (block) block->drawPadding(window, 0, 0, HOLD_POSITION_Y, HOLD_POSITION_X);
}