#ifndef COMPETITOR_HPP
#define COMPETITOR_HPP

#include <SFML/Network/TcpSocket.hpp>

#include "Monitor.hpp"

class Competitor :public Monitor {
private:
    sf::TcpSocket *recvSock;
public:
    Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpSocket *recvSock);
    ~Competitor();
    void start();
};

#endif