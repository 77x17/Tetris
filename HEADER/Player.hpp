#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <random>

class SoundManager;
class Monitor;
class CurrentBlockController;
class MovementController;

class Player{
private:
    Monitor* monitor;

    sf::Clock clock;
    sf::Clock movingClock;

    SoundManager *soundManager;
    MovementController *movementController;

    float volume;

    bool  collision;        // collision bottom (extra time to move and rotate)

protected:
    CurrentBlockController* curBlock;

public:
    Player(int X_COORDINATE, int Y_COORDINATE);
    ~Player();
    virtual void processEvents(const sf::Event &event);
    void autoDown();
    
    void resetComponent();
    bool isGameOver();

    void start();
    void restart();

    void draw(sf::RenderWindow* window);
};

#endif