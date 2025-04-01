#pragma once

#include <SFML/Window/Event.hpp>

#include "Player.hpp"

class Infor;
class Monitor_Multiplayer;
class MovementController_Multiplayer;

class Player_Multiplayer: public Player {
private:
    sf::TcpSocket socket;
    
public:
    Player_Multiplayer(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    Player_Multiplayer(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~Player_Multiplayer();

    void initialize();

    void setGameOver();

    void start(uint32_t seed);
    void restart(uint32_t seed);

    void sendCurBlock();

    void handleAddLine(uint8_t nLines);

    void ready(int& seed);

    void waitingComfirm();
};