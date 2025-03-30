#ifndef MOVEMENT_CONTROLLER_WITH_BOT_HPP
#define MOVEMENT_CONTROLLER_WITH_BOT_HPP

#include <SFML/Network/TcpSocket.hpp>
#include "MovementController.hpp"

class Monitor;
class CurrentBlockController;

class MovementControllerWithBot: public MovementController{
private:
    Monitor* competitor;    
public:
    MovementControllerWithBot(Monitor*, CurrentBlockController*);
    ~MovementControllerWithBot();
    void handlePut();
    void setCompetitor(Monitor* monitor);
};


#endif