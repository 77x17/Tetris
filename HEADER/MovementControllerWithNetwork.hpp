#ifndef MOVEMENT_CONTROLLER_WITH_NETWORK_HPP
#define MOVEMENT_CONTROLLER_WITH_NETWORK_HPP

#include <SFML/Network/TcpSocket.hpp>
#include "MovementController.hpp"

class Map;

class MovementControllerWithNetwork: public MovementController{
private:
    sf::TcpSocket socket;
public:
    MovementControllerWithNetwork(Monitor* monitor, CurrentBlockController* controller);
    ~MovementControllerWithNetwork();

    void handleUp(Map*);
    void handlePut();
    void handleHold();
};


#endif