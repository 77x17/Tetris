#ifndef MONITOR_HPP
#define MONITOR_HPP

// #include <ncurses.h>
#include <SFML/Graphics.hpp>

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

class Monitor {
protected:
    Hold* hold;
    CurrentBlock* curBlock;
    Map* map;
    Infor* infor;
    LinkListBlock* next;
    sf::RenderWindow *window;
    sf::Clock         clock;
    
public:
    Monitor(sf::RenderWindow *newWindow);
    ~Monitor();
    void processEvents();
    virtual void start()=0;
};

#endif