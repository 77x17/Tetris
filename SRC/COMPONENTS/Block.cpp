#include "Block.hpp"

void Block::draw(WINDOW* win, uint8_t y, uint8_t x, uint8_t type) {
    uint16_t tmp = getShape();
    char symbol = (type ? getSymbol() : '.');
    for (int i = 0; i < SIZEBLOCK; i++)
        if (getBit(tmp, i))
            mvwaddch(win, y, x, symbol);
}

void Block::rotateLeft() {if (--cur < 0) cur += 4;}
void Block::rotateRight() {if (++cur > 3) cur -= 4;}

IBlock::IBlock(){
    cur = 0; symbol = '#';
    shape = new uint16_t[4]; // nshape
    shape[0] = 0x0F00; shape[1] = 0x2222; shape[2] = 0x00F0; shape[3] = 0x4444;
}

IBlock::~IBlock() {
    delete[] shape;
}

uint16_t IBlock::getShape() { return shape[cur]; }
char IBlock::getSymbol() { return symbol; }

