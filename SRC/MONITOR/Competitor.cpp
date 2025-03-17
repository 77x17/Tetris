#include "Competitor.hpp"

#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <thread>

// Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE):Monitor(X_COORDINATE, Y_COORDINATE) {
//     curBlock->setter(next->updateNext());
//     curBlock->resetPosition(map);
// }

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed):Monitor(X_COORDINATE, Y_COORDINATE) {
    listenner.accept(recvSock);
    std::cout << "New client connected: " << recvSock.getRemoteAddress() << std::endl;

    next->setSeed(seed);
    recvSock.send(&seed, sizeof(seed));

    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
    mtx.unlock();
}

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port):Monitor(X_COORDINATE, Y_COORDINATE) {
    recvSock.connect(ipv4, port);
    std::cout << "New client connected: " << recvSock.getRemoteAddress() << std::endl;
    
    uint32_t seed = 0; std::size_t tmp=0;
    if (recvSock.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }
    next->setSeed(seed);

    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
    mtx.unlock();
}

Competitor::~Competitor() {
}

void Competitor::start() {
    sf::Event event;
    while (recvEvent(event)) {
        processEvents(event);
    }
    autoDown();
}

bool Competitor::recvEvent(sf::Event &event) {
    mtx.lock();
    if (pollEvent.empty()) {
        mtx.unlock();
        return false;
    }
    event = pollEvent.front(); pollEvent.pop();
    mtx.unlock();
    return true;
}

void Competitor::initPollEvent() {
    std::thread th([this](){
        while (true) {
            sf::Packet packet;
            if (recvSock.receive(packet) != sf::Socket::Done)
                throw std::runtime_error("Failed to receive event!");
    
            int eventType, keyCode;
            packet >> eventType >> keyCode;
    
            // Reconstruct the event
            sf::Event event;
            event.type = static_cast<sf::Event::EventType>(eventType);
            event.key.code = static_cast<sf::Keyboard::Key>(keyCode);
            
            mtx.lock();
            pollEvent.push(event);
            mtx.unlock();
        }
    });
    th.detach();
}