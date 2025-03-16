#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "Monitor.hpp"

class Player :public Monitor {
private:
    sf::TcpSocket sendSock;
public:
    Player(int X_COORDINATE, int Y_COORDINATE);
    Player(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner);
    Player(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~Player();
    
    void start();
    void sendEvent(const sf::Event &event);
};

#endif