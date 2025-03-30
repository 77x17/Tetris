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
    float volume;

protected:
    int X_COORDINATE;
    int Y_COORDINATE;
    Monitor* monitor;
    SoundManager *soundManager;
    MovementController *movementController;
    CurrentBlockController* curBlock;

public:
    Player(int X_COORDINATE, int Y_COORDINATE);

    virtual void initialize();

    virtual ~Player();
    virtual void processEvents(const sf::Event &event);
    void autoDown();

    void resetComponent();
    bool isGameOver();

    void start();
    void setTimer();
    void pauseTimer();
    void unPauseTimer();
    void restart();

    virtual void draw(sf::RenderWindow* window);
};

#endif