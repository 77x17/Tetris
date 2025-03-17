#ifndef COMPETITOR_HPP
#define COMPETITOR_HPP

#include <SFML/Network/TcpSocket.hpp>
#include <mutex>
#include <queue>
#include <random>

#include "Monitor.hpp"

class Competitor :public Monitor {
private:
    sf::TcpSocket recvSock;
    std::mutex mtx;
    std::queue<sf::Event> pollEvent;
    std::mt19937 gen;
public:
    // Competitor(int X_COORDINATE, int Y_COORDINATE);
    Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~Competitor();
    void start();
    void initPollEvent();
    bool recvEvent(sf::Event &event);
};

#endif