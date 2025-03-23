#include "CurrentBlock.hpp"

#include <iostream>

#include "Common.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "SoundManager.hpp"

CurrentBlock::CurrentBlock() : block(nullptr){}

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH_MAP / 2 - BLOCK_EDGE/2), posY(0){}

CurrentBlock::~CurrentBlock() { delete block; block = nullptr; }

bool CurrentBlock::isEmpty() { return (block == nullptr); }

Block* CurrentBlock::setter(Block* p) {
    Block* tmp = block; block = p;
    return tmp;
}

void CurrentBlock::freeAndSetter(Block* p) {
    delete block; block = p;
}

void CurrentBlock::setPosition(int x, int y, int sy) {
    posX = x; posY = y; shadowPosY = sy;
}

void CurrentBlock::swap(Hold* hold) {
    block = hold->interchange(block);
}

void CurrentBlock::draw(sf::RenderWindow *window, Map *map) const {
    map->drawCurrentBlock(window, block, posY, shadowPosY, posX);
}

bool CurrentBlock::isValid(Map* map) const {
    return map->isValid(block->getShape(), posY, posX);
}

uint8_t CurrentBlock::putIntoMap(Map* map) {
    return map->putBlockIntoMap(block, posY, posX);;
}

// char CurrentBlock::getTypeBlock() {
//     switch (block->getShapeID()) {
//         case 1:
//             return 'I';
//         case 2:
//             return 'L';
//         case 3:
//             return 'J';
//         case 4:
//             return 'O';
//         case 5:
//             return 'Z';
//         case 6:
//             return 'S';
//         case 7:
//             return 'T';
//         default:
//             throw std::invalid_argument("Invalid getTypeBlock()");
//     }

//     return char();
// }

// void CurrentBlock::compress(sf::Packet &packet) {
//     packet << block->getStateID() << posY << posX << shadowPosY;
// }

// void CurrentBlock::compressWithSpin(sf::Packet &packet) {
//     packet << block->getStateID() << posY << posX << shadowPosY << spin << (uint8_t)getTypeBlock();
// }