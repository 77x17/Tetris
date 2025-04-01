#ifndef CURRENTBLOCKCONTROLLER_MULTIPLAYER_HPP
#define CURRENTBLOCKCONTROLLER_MULTIPLAYER_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "CurrentBlockController.hpp"

class CurrentBlockController_Multiplayer: public CurrentBlockController {
public:
    CurrentBlockController_Multiplayer(Map* map);
    virtual ~CurrentBlockController_Multiplayer();

    virtual void createCurrentBlock();

    void compress(sf::Packet &packet);
    void compressWithSpin(sf::Packet &packet);
};

#endif