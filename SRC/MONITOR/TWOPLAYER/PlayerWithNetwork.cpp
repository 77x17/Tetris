#include "PlayerWithNetwork.hpp"

#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"
#include "Infor.hpp"
#include "Map.hpp"

#include <iostream>
#include <random>
#include <SFML/Network.hpp>

PlayerWithNetwork::PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listener, uint32_t seed):Player(X_COORDINATE, Y_COORDINATE) {
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    socket.send(&seed, sizeof(seed));
    next->setSeed(seed);
    start(seed);
}

PlayerWithNetwork::PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port):Player(X_COORDINATE, Y_COORDINATE) {
    socket.connect(ipv4, port);

    uint32_t seed = 0; std::size_t tmp=0;
    if (socket.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }
    next->setSeed(seed);
    start(seed);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
}

void PlayerWithNetwork::sendCurBlock() {
    sf::Packet packet; packet << CURBLOCK;
    curBlock->compress(packet);

    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
}

void PlayerWithNetwork::start(uint32_t seed) {
    resetComponent();
}

void PlayerWithNetwork::restart(uint32_t seed) {
    resetMonitor(seed);
    resetComponent();
}

void PlayerWithNetwork::handlePut() {
    sf::Packet packet; packet << PUT;
    curBlock->compressWithSpin(packet);

    // Player::handlePut();

    int nLines = curBlock->put(map);

    
    infor->removeLine(nLines);
    infor->playSoundRemoveLine(nLines, curBlock->spin, curBlock->getTypeBlock());
    
    if (nLines == 0) {
        std::random_device rd; int seed = rd();
        packet << seed;
        map->add(infor->getAndRemoveLineAdd(), seed);
    }

    resetComponent();
    if (curBlock->gameOver(map))
        setGameOver();

    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
}

void PlayerWithNetwork::handleHold() {
    sf::Packet packet; packet << HOLD;

    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    Player::handleHold();
}

void PlayerWithNetwork::handleUp() {
    Player::handleUp();
    if (curBlock->spin) {
        sf::Packet packet; packet << SPIN;
        if (socket.send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}

void PlayerWithNetwork::handleAddLine(uint8_t nLines, Infor* inforCompetitor) {
    infor->addLine(nLines, inforCompetitor);
    sf::Packet packet; packet << RECVLINE << nLines;
    inforCompetitor->compress(packet);

    if (socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
}