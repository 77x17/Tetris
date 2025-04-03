#include "Block.hpp"

#include "Common.hpp"
#include "CommonMap.hpp"

sf::Texture texture{ [] { sf::Texture t; t.loadFromFile("ASSETS/blocks/blocks.png"); return t; }() };
sf::Texture ghostTexture{ [] { sf::Texture t; t.loadFromFile("ASSETS/blocks/ghostBlocks.png"); return t; }() };

Block::Block() {
    stateID = 2;
    state = new uint16_t[4]();
}

Block::~Block() {
    delete[] state;
}

void Block::draw(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(Common::BLOCK_SIZE - 1, Common::BLOCK_SIZE - 1));
    block.setTexture(&texture);
    block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(offsetX + (j + x) * Common::BLOCK_SIZE + 1, offsetY + (i + y) * Common::BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

void Block::drawGhost(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(Common::BLOCK_SIZE - 1, Common::BLOCK_SIZE - 1));
    block.setTexture(&ghostTexture);
    block.setTextureRect(sf::IntRect((shapeID - 1) * 25, 0, 25, 25));

    block.setFillColor(sf::Color(150, 150, 150));
    
    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(offsetX + (j + x) * Common::BLOCK_SIZE + 1, offsetY + (i + y) * Common::BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

void Block::drawPadding(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(Common::BLOCK_SIZE - 1, Common::BLOCK_SIZE - 1));
    block.setTexture(&texture);
    block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(paddingX + offsetX + (j + x) * Common::BLOCK_SIZE + 1, paddingY + offsetY + (i + y) * Common::BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

void Block::drawBrownHold(sf::RenderWindow* window, int8_t y, int8_t x, int offsetY, int offsetX) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(Common::BLOCK_SIZE - 1, Common::BLOCK_SIZE - 1));
    block.setTexture(&texture);
    block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));
    block.setFillColor(sf::Color(255, 255, 255, 100));  // Màu xám trắng 

    uint16_t shape = getShape();
    for (int i = 0; i < BLOCK_EDGE; i++) {
        int mask = getLine(shape, i);
        for (int j = 0; j < BLOCK_EDGE; j++) {
            if (getBit(mask, j)) {
                block.setPosition(sf::Vector2f(paddingX + offsetX + (j + x) * Common::BLOCK_SIZE + 1, paddingY + offsetY + (i + y) * Common::BLOCK_SIZE + 1));
                window->draw(block);
            }
        }
    }
}

uint16_t Block::getRotate(int8_t times) {
    if (times < 0 || times >= 4) throw std::runtime_error("Have some problem with rotate times!");
    return state[(stateID + 4 - times) % 4]; 
}

void Block::rotate(int8_t times) {
    if (times < 0 || times >= 4) throw std::runtime_error("Have some problem with rotate times!");
    stateID = (stateID + 4 - times) % 4; 
}

uint16_t Block::getShape() { return state[stateID]; }
uint8_t Block::getStateID() { return stateID; }
void Block::setStateID(int x) { stateID = x; }

uint8_t Block::getShapeID() { return shapeID; }

void Block::resetState() { stateID = 2; }

IBlock::IBlock(): Block() {
    shapeID = 1;
    state[0] = 0x0F00; state[1] = 0x2222; state[2] = 0x00F0; state[3] = 0x4444;
    paddingX = Common::BLOCK_SIZE / 2;
    paddingY = -Common::BLOCK_SIZE;
}

LBlock::LBlock(): Block() {
    shapeID = 2;
    state[0] = 0x0710; state[1] = 0x2260; state[2] = 0x4700; state[3] = 0x3220;
    paddingX = Common::BLOCK_SIZE;
    paddingY = Common::BLOCK_SIZE / 2;
}

JBlock::JBlock(): Block() { 
    shapeID = 3;
    state[0] = 0x0740; state[1] = 0x6220; state[2] = 0x1700; state[3] = 0x2230;
    paddingX = Common::BLOCK_SIZE;
    paddingY = Common::BLOCK_SIZE / 2;
}

OBlock::OBlock(): Block() {
    shapeID = 4;
    state[0] = 0x0660; state[1] = 0x0660; state[2] = 0x0660; state[3] = 0x0660;
    paddingX = Common::BLOCK_SIZE / 2;
    paddingY = -Common::BLOCK_SIZE / 2;
}

ZBlock::ZBlock(): Block() {
    shapeID = 5;
    state[0] = 0x0360; state[1] = 0x2310; state[2] = 0x0360; state[3] = 0x2310;
    paddingX = Common::BLOCK_SIZE;
    paddingY = -Common::BLOCK_SIZE / 2;
}

SBlock::SBlock(): Block() {
    shapeID = 6;
    state[0] = 0x0630; state[1] = 0x1320; state[2] = 0x0630; state[3] = 0x1320;
    paddingX = Common::BLOCK_SIZE;
    paddingY = -Common::BLOCK_SIZE / 2;
}

TBlock::TBlock(): Block() {
    shapeID = 7;
    state[0] = 0x0720; state[1] = 0x2620; state[2] = 0x2700; state[3] = 0x2320;
    paddingX = Common::BLOCK_SIZE;
    paddingY = Common::BLOCK_SIZE / 2;
}
