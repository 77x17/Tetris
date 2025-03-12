#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Monitor.hpp"

class Player :public Monitor {
public:
    Player(sf::RenderWindow *window);
    void start();
    void update();
    void render();
};

#endif