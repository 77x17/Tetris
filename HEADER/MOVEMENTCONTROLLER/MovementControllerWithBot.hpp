#ifndef MOVEMENT_CONTROLLER_WITH_BOT_HPP
#define MOVEMENT_CONTROLLER_WITH_BOT_HPP

#include <SFML/Network/TcpSocket.hpp>
#include "MovementController.hpp"

class Map;
class Bot;

class MovementControllerWithBot: public MovementController{
private:
    Bot* competitor;    
public:
    MovementControllerWithBot(Monitor*, CurrentBlockController*);
    ~MovementControllerWithBot();
    void handlePut();
    void setCompetitor(Bot*);
};


#endif