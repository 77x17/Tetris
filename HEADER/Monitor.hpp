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

    bool moveLeft;
    bool moveRight;
    bool moveDown;

    bool gameOver;

    sf::Clock clock;            
    sf::Clock movingClock;      
    bool      collision;        // collision bottom (extra time to move and rotate)

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

public:
    Monitor(int X_COORDINATE, int Y_COORDINATE);
    virtual ~Monitor();
    void processEvents(const sf::Event &event);
    bool isGameOver();
    void restart(uint32_t seed = 0);
    void autoDown();
    void draw(sf::RenderWindow* window);
    virtual void start()=0;
};

#endif