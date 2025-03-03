#include "Tetris.hpp"

#include <iostream>

Tetris::Tetris(WINDOW* win):player(win){}

void Tetris::startGameOnePlayer() {
    player.start();
}