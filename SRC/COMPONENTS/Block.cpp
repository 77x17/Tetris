#include "Block.hpp"

#include "Common.hpp"


Block::Block() {
    stateID = 2;
    state = new uint16_t[4]();
}

Block::~Block() {
    delete[] state;
}

void Block::draw(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX, bool padding) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    block.setFillColor(sf::Color::Cyan);

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(paddingX * padding + offsetX + (j + x) * BLOCK_SIZE + 1, paddingY * padding + offsetY + (i + y) * BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

uint16_t Block::getRotateLeft() { return state[(stateID + 3) % 4]; }
uint16_t Block::getRotateRight() { return state[(stateID + 1) % 4]; }

void Block::rotateLeft() { stateID = (stateID + 3) % 4; }
void Block::rotateRight() { stateID = (stateID + 1) % 4; }

uint16_t Block::getShape() { return state[stateID]; }

void Block::resetState() { stateID = 2; }

IBlock::IBlock(): Block() {
    symbol = '#';
    state[0] = 0x0F00; state[1] = 0x2222; state[2] = 0x00F0; state[3] = 0x4444;
    paddingX = 0;
    paddingY = 0;
}

LBlock::LBlock(): Block() {
    symbol = '#';
    state[0] = 0x0E80; state[1] = 0xC440; state[2] = 0x2E00; state[3] = 0x4460;
    paddingX = 0;
    paddingY = 0;
}

JBlock::JBlock(): Block() { 
    symbol = '#';
    state[0] = 0x0E20; state[1] = 0x44C0; state[2] = 0x8E00; state[3] = 0x6440;
    paddingX = 0;
    paddingY = 0;
}

OBlock::OBlock(): Block() {
    symbol = '#';
    state[0] = 0x0660; state[1] = 0x0660; state[2] = 0x0660; state[3] = 0x0660;
    paddingX = 0;
    paddingY = 0;
}

ZBlock::ZBlock(): Block() {
    symbol = '#';
    state[0] = 0x0C60; state[1] = 0x2640; state[2] = 0x0C60; state[3] = 0x2640;
    paddingX = 0;
    paddingY = 0;
}

SBlock::SBlock(): Block() {
    symbol = '#';
    state[0] = 0x06C0; state[1] = 0x4620; state[2] = 0x06C0; state[3] = 0x4620;
    paddingX = 0;
    paddingY = 0;
}

TBlock::TBlock(): Block() {
    symbol = '#';
    state[0] = 0x0E40; state[1] = 0x4C40; state[2] = 0x4E00; state[3] = 0x4640;
    paddingX = 0;
    paddingY = 0;
}
