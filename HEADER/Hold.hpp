#ifndef HOLD_HPP
#define HOLD_HPP

// #include <ncurses.h>
#include <SFML/Graphics.hpp>

class Block; 

class Hold {
private:
    Block* block;
    bool holdPosible;
    sf::RenderWindow* window;
public:
    Hold(sf::RenderWindow* window);
    ~Hold();
    Block* interchange(Block* p);
    bool canHold();
    void lock();
    void unlock();
    void draw();
    void erase();
};

#endif