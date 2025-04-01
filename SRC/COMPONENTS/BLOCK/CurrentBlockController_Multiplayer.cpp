#include "CurrentBlockController_Multiplayer.hpp"

#include "CurrentBlock.hpp"
#include "CurrentBlock_Multiplayer.hpp"

CurrentBlockController_Multiplayer::CurrentBlockController_Multiplayer(Map* map) : CurrentBlockController(map) {}

void CurrentBlockController_Multiplayer::createCurrentBlock() {
    block = new CurrentBlock_Multiplayer();
}

CurrentBlockController_Multiplayer::~CurrentBlockController_Multiplayer() {}

void CurrentBlockController_Multiplayer::compress(sf::Packet &packet) {
    dynamic_cast<CurrentBlock_Multiplayer*>(block)->compress(packet);
}

void CurrentBlockController_Multiplayer::compressWithSpin(sf::Packet &packet) {
    dynamic_cast<CurrentBlock_Multiplayer*>(block)->compressWithSpin(packet);
}