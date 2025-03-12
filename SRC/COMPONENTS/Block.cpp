#include "Block.hpp"

#include "Common.hpp"

sf::Texture texture{ [] { sf::Texture t; t.loadFromFile("ASSETS/blocks/blocks.png"); return t; }() };

Block::Block() {
    stateID = 2;
    state = new uint16_t[4]();
}

Block::~Block() {
    delete[] state;
}

void Block::draw(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    block.setTexture(&texture);
    block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(offsetX + (j + x) * BLOCK_SIZE + 1, offsetY + (i + y) * BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

void Block::drawGhost(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    block.setTexture(&texture);
    block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));

    block.setFillColor(sf::Color(150, 150, 150));

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(offsetX + (j + x) * BLOCK_SIZE + 1, offsetY + (i + y) * BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

void Block::drawPadding(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    block.setTexture(&texture);
    block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(paddingX + offsetX + (j + x) * BLOCK_SIZE + 1, paddingY + offsetY + (i + y) * BLOCK_SIZE + 1));
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

uint8_t Block::getShapeID() { return shapeID; }

void Block::resetState() { stateID = 2; }

IBlock::IBlock(): Block() {
    shapeID = 0;
    state[0] = 0x0F00; state[1] = 0x2222; state[2] = 0x00F0; state[3] = 0x4444;
    paddingX = 0;
    paddingY = -BLOCK_SIZE;
}

LBlock::LBlock(): Block() {
    shapeID = 1;
    state[0] = 0x0710; state[1] = 0x2260; state[2] = 0x4700; state[3] = 0x3220;
    paddingX = BLOCK_SIZE / 2;
    paddingY = BLOCK_SIZE / 2;
}

JBlock::JBlock(): Block() { 
    shapeID = 2;
    state[0] = 0x0740; state[1] = 0x6220; state[2] = 0x1700; state[3] = 0x2230;
    paddingX = BLOCK_SIZE / 2;
    paddingY = BLOCK_SIZE / 2;
}

OBlock::OBlock(): Block() {
    shapeID = 3;
    state[0] = 0x0660; state[1] = 0x0660; state[2] = 0x0660; state[3] = 0x0660;
    paddingX = 0;
    paddingY = -BLOCK_SIZE / 2;
}

ZBlock::ZBlock(): Block() {
    shapeID = 4;
    state[0] = 0x0360; state[1] = 0x2310; state[2] = 0x0360; state[3] = 0x2310;
    paddingX = BLOCK_SIZE / 2;
    paddingY = -BLOCK_SIZE / 2;
}

SBlock::SBlock(): Block() {
    shapeID = 5;
    state[0] = 0x0630; state[1] = 0x1320; state[2] = 0x0630; state[3] = 0x1320;
    paddingX = BLOCK_SIZE / 2;
    paddingY = -BLOCK_SIZE / 2;
}

TBlock::TBlock(): Block() {
    shapeID = 6;
    state[0] = 0x0720; state[1] = 0x2620; state[2] = 0x2700; state[3] = 0x2320;
    paddingX = BLOCK_SIZE / 2;
    paddingY = BLOCK_SIZE / 2;
}
