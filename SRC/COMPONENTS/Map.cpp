#include "Map.hpp"

Map::Map() {
    for (int i = 0; i < HEIGHT; i++) map[i] = 0;
}

void Map::remove(uint8_t nLines) {
    
}

// the function adds nlines to map 
// returns a value indicating whether the map is valid or not
bool Map::add(uint8_t nLines) {
    return false;
}

void Map::display(WINDOW* win) {
    for (int i = 0; i < HEIGHT; i++) 
        for (int j = WIDTH - 1; j >= 0; j--)
            if (getBit(map[i], j))
                mvwaddch(win, i+1, j+1, '#');
            else mvwaddch(win, i+1, j+1, '.');
    wrefresh(win);
}