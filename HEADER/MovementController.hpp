#ifndef MOVEMENTCONTROLLER_HPP
#define MOVEMENTCONTROLLER_HPP


#include <SFML/Graphics.hpp>
#include <random>

#include "Monitor.hpp"

class CurrentBlockController;
class Map;
class Infor;
class Hold;
class LinkListBlock;

class MovementController{
private:
    // Monitor* monitor;
    sf::Clock clock;
    sf::Clock movingClock;

    bool  collision;        // collision bottom (extra time to move and rotate)

    bool  moveLeftSignal;
    bool  moveRightSignal;
    bool  moveDownSignal;

public:
    MovementController();
    ~MovementController();

    void resetComponent();
    
    void processEvents(const sf::Event &event, CurrentBlockController*, Monitor*);
    
    void handleLeft(CurrentBlockController*, Map*);
    void handleRight(CurrentBlockController*, Map*);
    void handleDown(CurrentBlockController*, Map*);
    void handleUp(CurrentBlockController*, Map*);
    void handleHardDrop(CurrentBlockController*,  Monitor*);
    void handlePut(CurrentBlockController*, Monitor*);
    void handleHold(CurrentBlockController*, Monitor*);

    void autoDown(CurrentBlockController*, Monitor*);
};

#endif