#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <SFML/Graphics.hpp>

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

enum MessageCode {CURBLOCK, PUT, HOLD, SPIN, RECVLINE,  GAMEOVER, RESTART};

class Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    bool gameOver;

protected:
    Hold             *hold;
    Map              *map;
    Infor            *infor;
    LinkListBlock    *next;

    void resetMonitor(uint32_t seed);
    
public:
    Monitor(int X_COORDINATE, int Y_COORDINATE);
    virtual ~Monitor();

    void setPosition(int X_COORDINATE, int Y_COORDINATE);
    
    bool isGameOver();
    void setGameOver();
    virtual void draw(sf::RenderWindow* window);
};

#endif