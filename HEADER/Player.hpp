#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <ncurses.h>

#include "Monitor.hpp"

class Player :public Monitor {
public:
    Player(WINDOW* win);
    void start();
};

#endif