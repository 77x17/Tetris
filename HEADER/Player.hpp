#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <ncurses.h>

#include "Screen.hpp"
#include "Monitor.hpp"

class Player :public Monitor {
public:
    Player();
    Player(WINDOW* win);

    void start();
};

#endif