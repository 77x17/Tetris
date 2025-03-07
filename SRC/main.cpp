#include "Common.hpp"
#include "Tetris.hpp"
#include "Block.hpp"

uint8_t Block::speed = 1;

int main(int argc, char **argv) {
    // initscr();
    // cbreak();
    // curs_set(0);
    // keypad(stdscr, TRUE);
    
    Tetris tetris;
    tetris.startGameOnePlayer();

    // getch();
    // refresh();
    
    // endwin();
    // return 0;
}