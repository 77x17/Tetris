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

CurrentBlockControllerWithNetwork::CurrentBlockControllerWithNetwork(Map* map, Infor* infor) : CurrentBlockController(map) {}

CurrentBlockControllerWithNetwork::~CurrentBlockControllerWithNetwork() {}

bool CurrentBlockControllerWithNetwork::rotateLeft() {
    
}

bool CurrentBlockControllerWithNetwork::rotateRight() {

    return true;
}

void CurrentBlockControllerWithNetwork::swap(Hold* hold) {
    sf::Packet packet; packet << HOLD;
    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    CurrentBlockController::swap(hold);
}

uint8_t CurrentBlockControllerWithNetwork::putIntoMap() {
    sf::Packet packet; packet << PUT;
    compressWithSpin(packet);
    
    int nLines = block->putIntoMap(map);
    
    if (nLines == 0) {
        std::random_device rd; int seed = rd();
        packet << seed;
        map->add(infor->getAndRemoveLineAdd(), seed);
    }

    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");

    infor->update(nLines, isJustSpin(), getTypeBlock());
    infor->playSound(nLines, isJustSpin(), getTypeBlock());
    infor->removeLine(nLines);

    // resetComponent();
    // if (curBlock->gameOver(map) && !isGameOver())
    //     setGameOver();
    return nLines;
}

void CurrentBlockControllerWithNetwork::compress(sf::Packet &packet) {
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY;
}

void CurrentBlockControllerWithNetwork::compressWithSpin(sf::Packet &packet) {
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY << isJustSpin() << (uint8_t)getTypeBlock();
}