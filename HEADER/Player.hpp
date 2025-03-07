#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

// #include <ncurses.h>
#include <SFML/Graphics.hpp>

#include "Monitor.hpp"

class Player :public Monitor {
public:
    Player(sf::RenderWindow *window);
    void start();
};

#endif