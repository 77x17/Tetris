#ifndef MOVEMENTCONTROLLER_HPP
#define MOVEMENTCONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <random>

class CurrentBlockController;
class Monitor;
class Map;
class LinkListBlock;

class MovementController{
private:
    Monitor* monitor; // dont't delete share pointer
    CurrentBlockController* curBlock; // don't delete share pointer

    sf::Clock clock;
    sf::Clock movingClock;

    bool  collision;        // collision bottom (extra time to move and rotate)

    bool  moveLeftSignal;
    bool  moveRightSignal;
    bool  moveDownSignal;

public:
    MovementController(Monitor* monitor, CurrentBlockController* contronller);
    ~MovementController();

    void resetComponent();
    
    void processEvents(const sf::Event &event);
    
    void handleLeft(Map*);
    void handleRight(Map*);
    void handleDown(Map*);
    void handleUp(Map*);
    void handleHardDrop();
    void handlePut();
    void handleHold();

    void autoDown();
};

#endif