#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <ncurses.h>
#include "Player.hpp"

class Tetris {
private:
    Player player;
public:
    Tetris(WINDOW* win);
    void startGameOnePlayer();
};
#endif