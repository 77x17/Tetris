#ifndef MOVEMENTCONTROLLER_HPP
#define MOVEMENTCONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <random>

class CurrentBlockController;
class Monitor;
class KeyConfiguration;

class MovementController{
private:
    int lines;
    float DROP_TIME;
    static constexpr float COLLISION_TIME      = 0.5f;
    static constexpr float COLLISION_DROP_TIME = 2.5f;

    static constexpr float DELAY_MOVING_TIME   = 175.0f;
    static constexpr float MOVING_TIME         = 30.0f;
    
    float movingTime;

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

    void speedUp();
};

#endif