#include "Map.hpp"

#include "CurrentBlock.hpp"
#include "Block.hpp"

Map::Map(WINDOW* _win) {
    for (int i = 0; i < HEIGHT; i++) map[i] = 0;
    win = derwin(_win, 25, 14, 0, 9);
    subbox = derwin(win, 22, 12, 2, 1);
    box(subbox, 0, 0);
    wrefresh(win);
    draw();
}

Map::~Map() {
    wclear(win); wclear(subbox);
    delwin(win); delwin(subbox);
}

void Map::remove(uint8_t nLines) {
    
}

// the function adds nlines to map 
// returns a value indicating whether the map is valid or not
bool Map::add(uint8_t nLines) {
    return false;
}

void Map::drawCur(Block* block, int Y, int X, uint8_t isOn) {
    block->draw(win, Y, X, isOn);
}

void Map::draw() {
    for (int i = 0; i < HEIGHT; i++) 
        for (int j = 0; j < WIDTH; j++)
            if (getBit(map[i], j))
                mvwaddch(subbox, i+1, j+1, '#');
            else mvwaddch(subbox, i+1, j+1, '.');
    wrefresh(subbox);
}

uint8_t Map::update(uint16_t shape, int Y, int X) {
    uint8_t cnt = 0;
    for (int i = 0; i < BLOCK_EDGE; i++) {
        map[Y + i] ^= ((MASK(BLOCK_EDGE) - 1) << X);
        cnt += (map[Y + i] & (MASK(WIDTH) - 1) == 0);
    }
    return cnt;
}

bool Map::isValid(uint16_t shape, int Y, int X) {
    for (int i = 0; i < BLOCK_EDGE; i++) 
        if ((getMask(map[i + Y], X) ^ getMask(shape, i)) != getMask(shape, i))
            return false;
    return true;
}