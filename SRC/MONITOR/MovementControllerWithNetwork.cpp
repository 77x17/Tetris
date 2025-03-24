#include "MovementControllerWithNetwork.hpp"

#include "SFML/Network.hpp"
#include "MessageCode.hpp"
#include "CurrentBlockControllerWithNetwork.hpp"

MovementControllerWithNetwork::MovementControllerWithNetwork(Monitor* monitor, CurrentBlockController* controller)
                                : MovementController(monitor, controller) {}

MovementControllerWithNetwork::~MovementControllerWithNetwork(){}

void MovementControllerWithNetwork::handleHold() {
    sf::Packet packet; packet << HOLD;
    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    MovementControllerWithNetwork::handleHold();
}

void MovementControllerWithNetwork::handlePut() {
    // sf::Packet packet; packet << PUT;
    // (curBlock->getCurrentBlock())->(packet);
    
    // int nLines = block->putIntoMap(map);
    
    // if (nLines == 0) {
    //     std::random_device rd; int seed = rd();
    //     packet << seed;
    //     map->add(infor->getAndRemoveLineAdd(), seed);
    // }

    // if (socket.send(packet) != sf::Socket::Done)
    //     throw std::runtime_error("Failed to send event!");

    // infor->update(nLines, isJustSpin(), getTypeBlock());
    // infor->playSound(nLines, isJustSpin(), getTypeBlock());
    // infor->removeLine(nLines);

    // resetComponent();
    // if (curBlock->gameOver(map) && !isGameOver())
    //     setGameOver();
}

void MovementControllerWithNetwork::handleUp(Map*) {
    // sf::Packet packet; packet << PUT;
    // (curBlock->getCurrentBlock())->(packet);
    
    // int nLines = block->putIntoMap(map);
    
    // if (nLines == 0) {
    //     std::random_device rd; int seed = rd();
    //     packet << seed;
    //     map->add(infor->getAndRemoveLineAdd(), seed);
    // }

    // if (socket.send(packet) != sf::Socket::Done)
    //     throw std::runtime_error("Failed to send event!");

    // infor->update(nLines, isJustSpin(), getTypeBlock());
    // infor->playSound(nLines, isJustSpin(), getTypeBlock());
    // infor->removeLine(nLines);

    // resetComponent();
    // if (curBlock->gameOver(map) && !isGameOver())
    //     setGameOver();
}