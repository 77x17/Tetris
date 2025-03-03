#include "CurrentBlock.hpp"

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH / 2 - BLOCK_EDGE/2), posY(0){}

void CurrentBlock::moveDown() {
    posY++;
}

void CurrentBlock::moveLeft() {
    posX--;
}

void CurrentBlock::moveRight() {
    posX++;
}

void CurrentBlock::draw(WINDOW* win, char c) {
    block->draw(win, posX, posY);
}