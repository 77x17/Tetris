#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Monitor.hpp"

class Player :public Monitor {
public:
    Player(int X_COORDINATE, int Y_COORDINATE);
    void start();
};

#endif