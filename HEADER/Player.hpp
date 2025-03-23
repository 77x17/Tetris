#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <random>

#include "Monitor.hpp"

class SoundManager;
// class Monitor;
class CurrentBlockController;
class MovementController;

class Player : public Monitor{
private:
    // Monitor* monitor;

    sf::Clock clock;
    sf::Clock movingClock;

    SoundManager *soundManager;
    MovementController *movementController;

    float volume;

    bool  collision;        // collision bottom (extra time to move and rotate)

protected:
    
    bool  moveLeftSignal;
    bool  moveRightSignal;
    bool  moveDownSignal;

    CurrentBlockController* curBlock;

    void handleLeft();
    void handleRight();
    void handleDown();
    void handleHardDrop();
    virtual void handlePut();
    virtual void handleHold();
    virtual void handleUp();

    void resetComponent();
public:
    Player(int X_COORDINATE, int Y_COORDINATE);
    ~Player();
    virtual void processEvents(const sf::Event &event);
    
    void autoDown();

    void start();
    void start(int seed);
    void restart();

    void draw(sf::RenderWindow* window);
};

#endif