#include "MovementController_Multiplayer.hpp"

#include "SFML/Network.hpp"
#include "MessageCode.hpp"
#include "Monitor_Multiplayer.hpp"
#include "Infor.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "LinkListBlock.hpp"
#include "CurrentBlockController_Multiplayer.hpp"
#include "CurrentBlock_Multiplayer.hpp"

MovementController_Multiplayer::MovementController_Multiplayer(Monitor* monitor, CurrentBlockController* controller, sf::TcpSocket& sock)
                                : MovementController(monitor, controller), socket(sock) {}

MovementController_Multiplayer::~MovementController_Multiplayer(){}

void MovementController_Multiplayer::handleHold() {
    sf::Packet packet; packet << HOLD;
    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    MovementController::handleHold();
}

void MovementController_Multiplayer::handlePut() {
    int nLines = monitor->putIntoMap(curBlock->getCurrentBlock());

    sf::Packet packet; packet << PUT;
    dynamic_cast<CurrentBlockController_Multiplayer*>(curBlock)->compressWithSpin(packet);
    if (nLines == 0) {
        std::random_device rd; int seed = rd();
        packet << seed;
        dynamic_cast<Monitor_Multiplayer*>(monitor)->mapReceiveLineFromCompetitor(seed);
    }
    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");

    // AllClearBUG
    monitor->removeNLines(nLines, curBlock->getCurrentBlock(), false);

    collision = false;
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
    if (curBlock->gameOver()) {
        sf::Packet packet; packet << GAMEOVER;
        if (socket.send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
        monitor->setGameOver();
        // if (socket.receive(packet) != sf::Socket::Done)
        //     throw std::runtime_error("Failed to receive event! FROM competitor handler process");
        // int messageCodeInt; packet >> messageCodeInt;
        // if (messageCodeInt != GAMEOVER)
        //     throw std::runtime_error("I don't understand message confirm! " + std::to_string(messageCodeInt));
        // // else std::cout << "COMFIRM SUCESSFULLY!\n";
    }
}

void MovementController_Multiplayer::handleRotateLeft() {
    MovementController::handleRotateLeft();
    if (curBlock->isJustSpin()) {
        sf::Packet packet; packet << SPIN;
        if (socket.send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}

void MovementController_Multiplayer::handleRotateRight() {
    MovementController::handleRotateRight();
    if (curBlock->isJustSpin()) {
        sf::Packet packet; packet << SPIN;
        if (socket.send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}

void MovementController_Multiplayer::handleRotate180() {
    MovementController::handleRotate180();
    if (curBlock->isJustSpin()) {
        sf::Packet packet; packet << SPIN;
        if (socket.send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}