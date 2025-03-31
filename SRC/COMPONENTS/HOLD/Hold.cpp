#include "Hold.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Monitor.hpp"

Hold::Hold() : block(nullptr), holdPosible(true) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

void Hold::setPosition(int x, int y, int w, int h) {
    HOLD_POSITION_X = x; HOLD_POSITION_Y = y;
    HOLD_WIDTH = w;      HOLD_HEIGHT = h;
}

Hold::~Hold() { 
    delete block;
    block = nullptr;
}

void Hold::reset() {
    unlock();
    delete block;
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
    sf::Text text("HOLD", font, Common::BLOCK_SIZE - Common::BLOCK_SIZE / 3);
    text.setPosition(HOLD_POSITION_X, HOLD_POSITION_Y - Common::BLOCK_SIZE - Common::BLOCK_SIZE / 6);
    window->draw(text);
    
    sf::RectangleShape line;
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White

    // Upper - lower line
    line.setSize(sf::Vector2f(HOLD_WIDTH * Common::BLOCK_SIZE + Common::WIDTH_BORDER + Common::WIDTH_BORDER, Common::WIDTH_BORDER));
    line.setPosition(HOLD_POSITION_X - Common::WIDTH_BORDER, HOLD_POSITION_Y - Common::WIDTH_BORDER);
    window->draw(line);
    line.setPosition(HOLD_POSITION_X - Common::WIDTH_BORDER, HOLD_POSITION_Y + HOLD_HEIGHT * Common::BLOCK_SIZE);
    window->draw(line);
    
    // Left - right line
    line.setSize(sf::Vector2f(1, HOLD_HEIGHT * Common::BLOCK_SIZE));
    line.setPosition(HOLD_POSITION_X                          , HOLD_POSITION_Y);
    window->draw(line);
    line.setPosition(HOLD_POSITION_X + HOLD_WIDTH * Common::BLOCK_SIZE, HOLD_POSITION_Y);
    window->draw(line);
}

void Hold::draw(sf::RenderWindow *window) {
    if (block) {
        if (holdPosible) {
            block->drawPadding(window, 0, 0, HOLD_POSITION_Y, HOLD_POSITION_X);
        } 
        else {
            block->drawBrownHold(window, 0, 0, HOLD_POSITION_Y, HOLD_POSITION_X);
        }
    }
}