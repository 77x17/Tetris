#include <ncurses.h>
#include <iostream>

int main(int argc, char **argv) {
    initscr();

    getch();
    endwin();
    return 0;
}