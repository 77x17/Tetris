#include "Common.hpp"
#include "Tetris.hpp"

uint8_t Block::speed = 1;

int main(int argc, char **argv) {
    initscr();
    cbreak();
    curs_set(0);
    Tetris tetris;
    tetris.startGameOnePlayer();

    getch();
    refresh();
    
    endwin();
    return 0;
}