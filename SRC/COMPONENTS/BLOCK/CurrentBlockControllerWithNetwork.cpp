#include "CurrentBlockControllerWithNetwork.hpp"

#include <iostream>
#include <random>

#include "Common.hpp"
#include "CurrentBlock.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "Infor.hpp"
#include "Block.hpp"
#include "MessageCode.hpp"

CurrentBlockControllerWithNetwork::CurrentBlockControllerWithNetwork(Map* map) : CurrentBlockController(map) {}

CurrentBlockControllerWithNetwork::~CurrentBlockControllerWithNetwork() {}

void CurrentBlockControllerWithNetwork::compress(sf::Packet &packet) {
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY;
}

void CurrentBlockControllerWithNetwork::compressWithSpin(sf::Packet &packet) {
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY << isJustSpin() << (uint8_t)getTypeBlock();
}