#include "Block.hpp"
#include "Common.hpp"


void Block::draw(WINDOW* win, uint8_t y, uint8_t x, uint8_t isOn) {
    char ch = (isOn ? symbol : '.');
    uint16_t shape = getShape();
    for (int i = 0; i < SIZEBLOCK; i++)
        if (getBit(shape, i))
            mvwaddch(win, y + i / BLOCK_EDGE, x + i % BLOCK_EDGE, ch);
    wrefresh(win);
}

uint16_t Block::getRotateLeft() { return state[(stateID + 3) % 4]; }
uint16_t Block::getRotateRight() { return state[(stateID + 1)% 4]; }

void Block::rotateLeft() { stateID = (stateID + 3) % 4; }
void Block::rotateRight() { stateID = (stateID + 1) % 4; }

uint16_t Block::getShape() { return state[stateID]; }

IBlock::IBlock() {
    stateID = 0; symbol = '#';
    state = new uint16_t[4]();
    state[0] = 0x0F00; state[1] = 0x2222; state[2] = 0x00F0; state[3] = 0x4444;
}

IBlock::~IBlock() {
    delete[] state;
}