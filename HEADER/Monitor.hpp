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

protected:
    Hold             *hold;
    CurrentBlock     *curBlock;
    Map              *map;
    Infor            *infor;
    LinkListBlock    *next;
    sf::RenderWindow *window;
    sf::Clock         clock;
    sf::Clock         collisionClock;
    bool              collision;
    
    void handleLeft();
    void handleRight();
    void handleDown();
    void handleUp();
    void handleHardDrop();
    void handleHold();

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