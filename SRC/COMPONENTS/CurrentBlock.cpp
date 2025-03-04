#include "CurrentBlock.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Map.hpp"

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH / 2 - BLOCK_EDGE/2), posY(0){}

CurrentBlock::~CurrentBlock() {
    delete block;
}

bool CurrentBlock::moveDown(Map* map) {
    posY++;
    if (!isValid(map)) { posY--; return false; }
    map->drawCur(block, posY - 1, posX, false);
    map->drawCur(block, posY, posX, true);
    return true;
}

bool CurrentBlock::moveLeft(Map* map) {
    posX--;
    if (!isValid(map)) { posX++; return false; }
    map->drawCur(block, posY, posX + 1, false);
    map->drawCur(block, posY, posX, true);
    return true;
}

bool CurrentBlock::moveRight(Map* map) {
    posX++;
    if (!isValid(map)) { posX--; return false; }
    map->drawCur(block, posY, posX - 1, false);
    map->drawCur(block, posY, posX, true);
    return true;
}

void CurrentBlock::draw(Map* map) {
    map->drawCur(block, posY, posX, 1);
}

bool CurrentBlock::isValid(Map* map) {
    if (posX <= 1 || posX > WIDTH) return false;
    if (posY > HEIGHT) return false;
    return map->isValid(block->getShape(), posY, posX);
}