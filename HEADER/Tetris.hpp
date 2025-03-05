#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <ncurses.h>

class Player;

class Tetris {
private:
    Player* player;
    WINDOW* win;
public:
    Tetris();
    ~Tetris();
    void startGameOnePlayer();
};
#endif