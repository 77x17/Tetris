#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <SFML/Graphics.hpp>

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

class Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;

    sf::Clock clock;            
    sf::Clock movingClock;      
    bool      collision;        // collision bottom (extra time to move and rotate)
    bool      spin;             // check block spin 

    void handleLeft();
    void handleRight();
    void handleDown();
    void handleUp();
    void handlePut();
    void handleHardDrop();
    void handleHold();

protected:
    Hold             *hold;
    CurrentBlock     *curBlock;
    Map              *map;
    Infor            *infor;
    LinkListBlock    *next;
    sf::RenderWindow *window;

public:
    Monitor(sf::RenderWindow *newWindow, int X_COORDINATE, int Y_COORDINATE);
    virtual ~Monitor();
    void processEvents();
    void restart();
    void update();
    void render();
    virtual void start()=0;
};

#endif