#include "CurrentBlock.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Map.hpp"

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH / 2 - BLOCK_EDGE/2), posY(0){}

CurrentBlock::~CurrentBlock() {
    delete block; block = nullptr;
}

bool CurrentBlock::moveDown(Map* map) {
    posY++;
    if (!isValid(block->getShape(), map)) { posY--; return false; }
    map->drawCur(block, posY - 1, posX, false);
    map->drawCur(block, posY, posX, true);
    return true;
}

bool CurrentBlock::moveLeft(Map* map) {
    posX--;
    if (!isValid(block->getShape(), map)) { posX++; return false; }
    map->drawCur(block, posY, posX + 1, false);
    map->drawCur(block, posY, posX, true);
    return true;
}

bool CurrentBlock::moveRight(Map* map) {
    posX++;
    if (!isValid(block->getShape(), map)) { posX--; return false; }
    map->drawCur(block, posY, posX - 1, false);
    map->drawCur(block, posY, posX, true);
    return true;
}

bool CurrentBlock::rotateLeft(Map* map) {
    if (!isValid(block->getRotateLeft(), map)) return false;
    map->drawCur(block, posY, posX, false);
    block->rotateLeft();
    map->drawCur(block, posY, posX, true);
    return true;
}

bool CurrentBlock::rotateRight(Map* map) {

    return true;
}

void CurrentBlock::draw(Map* map) {
    map->drawCur(block, posY, posX, 1);
}

bool CurrentBlock::isValid(uint16_t shape, Map* map) {
    return map->isValid(shape, posY, posX);
}

uint8_t CurrentBlock::put(Map* map) {
    return map->update(block->getShape(), posY, posX);
}