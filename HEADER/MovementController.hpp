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
    sf::Clock clock;
    sf::Clock movingClock;

    bool  collision;        // collision bottom (extra time to move and rotate)

    bool  moveLeftSignal;
    bool  moveRightSignal;
    bool  moveDownSignal;

protected:
    Monitor* monitor; // dont't delete share pointer
    CurrentBlockController* curBlock; // don't delete share pointer

public:
    MovementController(Monitor* monitor, CurrentBlockController* controller);
    ~MovementController();

    void resetComponent();
    
    void processEvents(const sf::Event &event);
    
    void handleLeft(Map*);
    void handleRight(Map*);
    void handleDown(Map*);
    void handleHardDrop();

    virtual void handleUp(Map*);
    virtual void handlePut();
    virtual void handleHold();

    void autoDown();
};

#endif