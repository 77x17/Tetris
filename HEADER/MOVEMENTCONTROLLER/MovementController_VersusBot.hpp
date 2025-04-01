#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "MovementController.hpp"

class Monitor;
class CurrentBlockController;

class MovementController_VersusBot: public MovementController{
private:
    Monitor* competitor;    
public:
    MovementController_VersusBot(Monitor*, CurrentBlockController*);
    ~MovementController_VersusBot();
    void handlePut();
    void setCompetitor(Monitor* monitor);
};