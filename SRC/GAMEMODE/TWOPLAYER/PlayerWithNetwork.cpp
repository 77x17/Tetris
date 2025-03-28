#include "PlayerWithNetwork.hpp"

#include "CurrentBlockController.hpp"
#include "LinkListBlock.hpp"
#include "Infor.hpp"
#include "Map.hpp"
#include "MonitorForTwoPlayer.hpp"
#include "MovementControllerWithNetwork.hpp"
#include "MessageCode.hpp"
#include "SoundManager.hpp"
#include "Hold.hpp"

#include <iostream>
#include <random>
#include <SFML/Network.hpp>

PlayerWithNetwork::PlayerWithNetwork(int x, int y, sf::TcpListener &listener, uint32_t seed): Player(x, y) {
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    socket.send(&seed, sizeof(seed));
    initialize(); monitor->setNewSeed(seed);
    start(seed);
}

PlayerWithNetwork::PlayerWithNetwork(int x, int y, const char* ipv4, int port): Player(x, y){
    socket.connect(ipv4, port);
    uint32_t seed = 0; std::size_t tmp=0;
    if (socket.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }
    initialize(); monitor->setNewSeed(seed);
    start(seed);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
}

void PlayerWithNetwork::initialize() {
    monitor = new MonitorForTwoPlayer(X_COORDINATE, Y_COORDINATE);
    monitor = new MonitorForTwoPlayer(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlockController(monitor->getMap());
    soundManager = new SoundManager();
    movementController = new MovementControllerWithNetwork(monitor, curBlock, &socket);
}

PlayerWithNetwork::~PlayerWithNetwork() {}

void PlayerWithNetwork::setGameOver() {
    dynamic_cast<MovementControllerWithNetwork*>(movementController)->setGameOver();
}

void PlayerWithNetwork::sendCurBlock() {
    sf::Packet packet; packet << CURBLOCK;
    curBlock->compress(packet);
    
    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
} 

void PlayerWithNetwork::start(uint32_t seed) {
    restart(seed);
}

void PlayerWithNetwork::restart(uint32_t seed) {
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter((monitor->getNext())->updateNext());
    monitor->unlockHold();
}

void PlayerWithNetwork::handleAddLine(uint8_t nLines) {
    dynamic_cast<MonitorForTwoPlayer*>(monitor)->inforReceiveLineFromCompetitor(nLines);
    sf::Packet packet; packet << RECVLINE << nLines;

    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
}

void PlayerWithNetwork::ready(int& seed) {
    sf::Packet packet; packet << RESTART << seed;
    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    restart(seed);
}

void PlayerWithNetwork::waitingComfirm() {
    sf::Packet packet;
    if (socket.receive(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to receive event! FROM competitor handler process");
    int messageCodeInt;
    packet >> messageCodeInt;
    if (messageCodeInt != GAMEOVER) 
        throw std::runtime_error("I don't understand message confirm! " + std::to_string(messageCodeInt));
    else std::cout << "COMFIRM SUCESSFULLY!\n";
}