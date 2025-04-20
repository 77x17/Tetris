#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "MovementController.hpp"

class Map;

class MovementController_Multiplayer: public MovementController{
private:
    sf::TcpSocket& socket;
public:
    MovementController_Multiplayer(Monitor* monitor, CurrentBlockController* controller, sf::TcpSocket& socket);
    ~MovementController_Multiplayer();

    void handleRotateLeft();
    void handleRotateRight();
    void handleRotate180();
    void handlePut();
    void handleHold();
    
    void setGameOver();
};
