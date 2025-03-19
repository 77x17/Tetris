#ifndef COMPETITOR_HPP
#define COMPETITOR_HPP

#include <SFML/Network/TcpSocket.hpp>
#include <mutex>
#include <queue>
#include <random>

#include "Monitor.hpp"

class Block;
class SoundManager;
class PlayerWithNetwork;

class Competitor : public Monitor {
private:
    sf::TcpSocket recvSock;
    std::mutex mtx;
    
    Block* curBlock;
    uint8_t posX, posY, posYShadow;
    SoundManager *soundManager;

    std::queue<sf::Event> pollEvent;
    std::mt19937 gen;
public:
    // Competitor(int X_COORDINATE, int Y_COORDINATE);
    Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~Competitor();
    void start(PlayerWithNetwork* &player);
    // void initPollEvent();
    void draw(sf::RenderWindow* window);

    void handleAddLine(uint8_t nLines);
};

#endif