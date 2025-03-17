#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "Monitor.hpp"

#include <random>

class Player :public Monitor {
private:
    bool moveLeft;
    bool moveRight;
    bool moveDown;

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

public:
    Player(int X_COORDINATE, int Y_COORDINATE);
    ~Player();
    void processEvents(const sf::Event &event);
    void autoDown();

    void start(uint32_t seed);
    void restart(uint32_t seed);
};

#endif