#include "CurrentBlock.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Hold.hpp"

CurrentBlock::CurrentBlock() : block(nullptr) {}

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH / 2 - BLOCK_EDGE/2), posY(0){}

CurrentBlock::~CurrentBlock() {
    delete block; block = nullptr;
}

bool CurrentBlock::isEmpty() {
    return (block == nullptr);
}

void CurrentBlock::setter(Block* p) {
    block = p;
}

bool CurrentBlock::resetPosition(Map *map) {
    posX = WIDTH / 2 - BLOCK_EDGE / 2; posY = 0;

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::moveDown(Map* map) {
    if (not map->isValid(block->getShape(), posY + 1, posX)) 
        return false; 

    posY++;

    return true;
}

bool CurrentBlock::moveLeft(Map* map) {
    if (not map->isValid(block->getShape(), posY, posX - 1)) 
        return false; 

    posX--;

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::moveRight(Map* map) {
    if (not map->isValid(block->getShape(), posY, posX + 1)) 
        return false; 
    
    posX++;

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::hardDrop(Map *map) {
    posY = shadowPosY;

    return true;
}

void CurrentBlock::shadowHardDrop(Map *map) {
    shadowPosY = posY;
    while (map->isValid(block->getShape(), shadowPosY + 1, posX)) {
        shadowPosY++;
    }
}

bool CurrentBlock::collisionBottom(Map *map) {
    return not map->isValid(block->getShape(), posY + 1, posX);
}

bool CurrentBlock::rotateLeft(Map* map) {
    if (!isValid(block->getRotateLeft(), map)) return false;
    
    block->rotateLeft();

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::rotateRight(Map* map) {

    return true;
}

void CurrentBlock::swap(Hold* hold) {
    block = hold->interchange(block);
}

void CurrentBlock::draw(sf::RenderWindow *window) {
    block->draw(window, posY, posX, GRID_POSITION_Y, GRID_POSITION_X, false);
    block->draw(window, shadowPosY, posX, GRID_POSITION_Y, GRID_POSITION_X, false);
}

bool CurrentBlock::isValid(uint16_t shape, Map* map) {
    return map->isValid(shape, posY, posX);
}

uint8_t CurrentBlock::put(Map* map) {
    return map->update(block, posY, posX);
}