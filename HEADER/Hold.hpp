#ifndef HOLD_HPP
#define HOLD_HPP

// #include <ncurses.h>
#include <SFML/Graphics.hpp>

class Block; 

class Hold {
private:
    Block* block;
    bool holdPosible;
public:
    Hold();
    ~Hold();
    Block* interchange(Block* p);
    bool canHold();
    void lock();
    void unlock();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif