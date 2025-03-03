#include "Monitor.hpp"
#include "Map.hpp"
#include "Common.hpp"
#include "Piece.hpp"
#include "Player.hpp"
#include "Screen.hpp"
#include "SetPiece.hpp"
#include "Tetris.hpp"

SetPiece Monitor::setPiece(1);
uint8_t Piece::speed = 1;

int main(int argc, char **argv) {
    initscr();
    cbreak();
    
    int h, w;
    getmaxyx(stdscr, h, w);

    WINDOW *win = newwin(25, 30, h/2 - 25/2, w/2 - 30/2); // WINDOW * win = newwin(nlines, ncols, y0, x0);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    Tetris tetris(win);
    tetris.startGameOnePlayer();

    getch();
    refresh();
    
    delwin(win);
    endwin();
    return 0;
}