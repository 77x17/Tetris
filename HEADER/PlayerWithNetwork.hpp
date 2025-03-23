#ifndef PLAYERWITHNETWORK_HPP
#define PLAYERWITHNETWORK_HPP

#include <SFML/Window/Event.hpp>

#include "Player.hpp"

class Infor;

class PlayerWithNetwork: public Player {
private:
    sf::TcpSocket socket;

public:
    PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);

    void setGameOver();

    void start(uint32_t seed);
    void restart(uint32_t seed);

    void sendCurBlock();

    void handlePut();
    void handleHold();
    void handleUp();
    void handleAddLine(uint8_t nLines);

    void ready(int& seed);

    void waitingComfirm();
};

#endif