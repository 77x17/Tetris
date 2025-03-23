#ifndef HOLD_HPP
#define HOLD_HPP

#include <SFML/Graphics.hpp>

class Block;
#include "Monitor.hpp"

class Hold {
private:
    int HOLD_POSITION_X;
    int HOLD_POSITION_Y;
    int HOLD_WIDTH;
    int HOLD_HEIGHT;
    
    Block*   block;
    bool     holdPosible;
    sf::Font font;

    void setPosition(int HOLD_POSITION_X, int HOLD_POSITION_Y, int HOLD_WIDTH, int HOLD_HEIGHT);

public:
    Hold();
    ~Hold();

    void reset();
    
    Block* interchange(Block* p);
    bool canHold();
    void lock();
    void unlock();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);

    friend void Monitor::setPosition(int X_COORDINATE, int Y_COORDINATE);
};

#endif