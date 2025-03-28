#ifndef MOVEMENTCONTROLLER_HPP
#define MOVEMENTCONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <random>

class CurrentBlockController;
class Monitor;
class KeyConfiguration;

class MovementController{
private:
    sf::Clock clock;
    sf::Clock movingClock;
    KeyConfiguration* key;
protected:
    bool  collision;        // collision bottom (extra time to move and rotate)

    bool  moveLeftSignal;
    bool  moveRightSignal;
    bool  moveDownSignal;

protected:
    Monitor* monitor; // dont't delete share pointer
    CurrentBlockController* curBlock; // don't delete share pointer

public:
    MovementController(Monitor* monitor, CurrentBlockController* controller);
    virtual ~MovementController();

    void resetComponent();
    
    void processEvents(const sf::Event &event);
    
    void handleLeft();
    void handleRight();
    void handleDown();
    void handleHardDrop();

    virtual void handleRotateLeft();
    virtual void handleRotateRight();
    virtual void handleRotate180();
    virtual void handlePut();
    virtual void handleHold();

    void autoDown();
};

#endif