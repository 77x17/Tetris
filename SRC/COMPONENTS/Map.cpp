#include "Map.hpp"

#include "CurrentBlock.hpp"
#include "Block.hpp"

#define OFFSETX 4
#define OFFSETY 0
#define OFFRIGHT 12
#define NUMOFFSET 2
#define REALWIDTH 14
// **--------** REALWIDTH
// ** NUMOFFSET 
// **-------- OFFRIGHT

#define EMPTYLINE() (FULLMASK(NUMOFFSET) ^ (FULLMASK(NUMOFFSET) << OFFRIGHT))

Map::Map(WINDOW* _win) {
    for (int i = 0; i < HEIGHT; i++) map[i] = EMPTYLINE();
    map[HEIGHT] = FULLMASK(REALWIDTH);

    win = derwin(_win, 25, 20, 3, 9); // modify
    subbox = derwin(win, 21, 12, 4, 3);
    wborder(subbox, '|', '|', ' ', '-', '|', '|', '+', '+');
    wrefresh(subbox);
    draw();
}

Map::~Map() {
    wclear(win); wclear(subbox);
    delwin(subbox); subbox = nullptr;
    delwin(win); win = nullptr;
}

void Map::remove(uint8_t pos) {
    for (int i = pos - 1; i >= 0; i--) 
        map[i + 1] = map[i];
    map[0] = EMPTYLINE();
}

// the function adds nlines to map 
// returns a value indicating whether the map is valid or not
bool Map::add(uint8_t nLines) {
    return false;
}

void Map::drawCur(Block* block, int Y, int X, uint8_t isOn) {
    block->draw(win, Y + OFFSETY, X + OFFSETX, isOn);
}

void Map::draw() {
    for (int i = 0; i < HEIGHT; i++) 
        for (int j = 0; j < WIDTH; j++)
            if (getBit(map[i], j+NUMOFFSET))
                mvwaddch(win, i + OFFSETY, j + OFFSETX, '#');
            else if (i + OFFSETY >= BLOCK_EDGE)
                mvwaddch(win, i+OFFSETY, j+OFFSETX, '.');
    wrefresh(win);
}

uint8_t Map::update(uint16_t shape, int Y, int X) {
    uint8_t cnt = 0;
    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < HEIGHT) {
        map[Y + i - cnt] ^= (getLine(shape, i) << (X + NUMOFFSET));
        if (((map[Y + i - cnt] & FULLMASK(REALWIDTH)) ^ FULLMASK(REALWIDTH)) == 0) {
            remove(Y + i - cnt);
            cnt++;
        }
    }
    draw();
    return cnt;
}

bool Map::isValid(uint16_t shape, int Y, int X) {
    if (X < -NUMOFFSET || X + BLOCK_EDGE > WIDTH + NUMOFFSET) return false;

    for (int i = 0; i < BLOCK_EDGE; i++)
        if (((getMask(map[i + Y], X + NUMOFFSET) ^ getLine(shape, i)) & getMask(map[i + Y], X + NUMOFFSET)) != getMask(map[i + Y], X + NUMOFFSET))
            return false;
    return true;
}