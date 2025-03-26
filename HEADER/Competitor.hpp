#ifndef COMPETITOR_HPP
#define COMPETITOR_HPP

#include <SFML/Network/TcpSocket.hpp>
#include <mutex>
#include <queue>
#include <random>

#include "Monitor.hpp"

class CurrentBlock;
class SoundManager;
class PlayerWithNetwork;

class Competitor{
private:
    Monitor* monitor;
    sf::TcpSocket socket;

    std::mutex mtx;
    
    CurrentBlock* curBlock;
    SoundManager *soundManager;

    std::mt19937 gen;
public:
    // Competitor(int X_COORDINATE, int Y_COORDINATE);
    Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~Competitor();
    void start(PlayerWithNetwork* &player);
    void ready(int& seed);

    bool isGameOver();

    void draw(sf::RenderWindow* window);
};

#endif