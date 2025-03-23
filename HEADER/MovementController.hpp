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
    
    void processEvents(const sf::Event &event, CurrentBlockController*, Map*, Infor*, Hold*, LinkListBlock*);
    
    void handleLeft(CurrentBlockController*, Map*);
    void handleRight(CurrentBlockController*, Map*);
    void handleDown(CurrentBlockController*, Map*);
    void handleHardDrop(CurrentBlockController*,  Map*, Hold*, Infor*, LinkListBlock*);
    void handlePut(CurrentBlockController*, Map*, Hold*, Infor*, LinkListBlock*);
    void handleHold(CurrentBlockController*, Map*, Hold*, LinkListBlock*);
    void handleUp(CurrentBlockController*, Map*);

    void autoDown(CurrentBlockController*, Map*, Infor*, Hold*, LinkListBlock*);
};

#endif