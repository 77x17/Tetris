#include "CurrentBlock.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Hold.hpp"

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH / 2 - BLOCK_EDGE/2), posY(0){}

CurrentBlock::~CurrentBlock() {
    delete block; block = nullptr;
}

bool CurrentBlock::isEmpty() {
    return (block == nullptr);
}

void CurrentBlock::setter(Block* p) {
    block = p;
    posX = WIDTH / 2 - BLOCK_EDGE/2; posY = 0;
}

bool CurrentBlock::moveDown(Map* map) {
    posY++;
    if (!isValid(block->getShape(), map)) { posY--; return false; }
    map->eraseCur(block, posY - 1, posX);
    map->drawCur(block, posY, posX);
    return true;
}

bool CurrentBlock::moveLeft(Map* map) {
    posX--;
    if (!isValid(block->getShape(), map)) { posX++; return false; }
    map->eraseCur(block, posY, posX + 1);
    map->drawCur(block, posY, posX);
    return true;
}

bool CurrentBlock::moveRight(Map* map) {
    posX++;
    if (!isValid(block->getShape(), map)) { posX--; return false; }
    map->eraseCur(block, posY, posX - 1);
    map->drawCur(block, posY, posX);
    return true;
}

bool CurrentBlock::rotateLeft(Map* map) {
    if (!isValid(block->getRotateLeft(), map)) return false;
    map->eraseCur(block, posY, posX);
    block->rotateLeft();
    map->drawCur(block, posY, posX);
    return true;
}

bool CurrentBlock::rotateRight(Map* map) {

    return true;
}

void CurrentBlock::swap(Hold* hold) {
    this->setter(hold->interchange(block));
}

void CurrentBlock::draw(Map* map) {
    map->drawCur(block, posY, posX);
}

void CurrentBlock::erase(Map* map) {
    map->eraseCur(block, posY, posX);
}

bool CurrentBlock::isValid(uint16_t shape, Map* map) {
    return map->isValid(shape, posY, posX);
}

uint8_t CurrentBlock::put(Map* map) {
    return map->update(block->getShape(), posY, posX);
}