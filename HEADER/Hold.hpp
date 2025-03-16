#ifndef HOLD_HPP
#define HOLD_HPP

#include <SFML/Graphics.hpp>

class Block; 

class Hold {
private:
    int HOLD_POSITION_X;
    int HOLD_POSITION_Y;
    int HOLD_WIDTH;
    int HOLD_HEIGHT;
    
    Block*   block;
    bool     holdPosible;
    sf::Font font;

public:
    Hold(int HOLD_POSITION_X, int HOLD_POSITION_Y, int HOLD_WIDTH, int HOLD_HEIGHT);
    ~Hold();
    Block* interchange(Block* p);
    bool canHold();
    void lock();
    void unlock();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif