#ifndef MOVEMENT_CONTROLLER_WITH_BOT_HPP
#define MOVEMENT_CONTROLLER_WITH_BOT_HPP

#include <SFML/Network/TcpSocket.hpp>
#include "MovementController.hpp"

class Map;

class MovementControllerWithBot: public MovementController{
private:
public:
    MovementControllerWithBot(Monitor*, CurrentBlockController*);
    ~MovementControllerWithBot();
    void handlePut();
};


#endif