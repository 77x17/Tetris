#ifndef PLAYERWITHNETWORK_HPP
#define PLAYERWITHNETWORK_HPP

#include "Player.hpp"

class PlayerWithNetwork: public Player {
private:
    sf::TcpSocket socket;
public:
    PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);

    void start(uint32_t seed);
    void restart(uint32_t seed);
};

#endif