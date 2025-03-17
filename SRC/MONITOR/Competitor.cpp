#include "Competitor.hpp"

#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "Block.hpp"

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
    std::cout << "New client connected: " << recvSock.getRemoteAddress() << " SEED:" << seed << std::endl;

    next->setSeed(seed);
    recvSock.send(&seed, sizeof(seed));
    curBlock = nullptr;
    mtx.unlock();
}

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port):Monitor(X_COORDINATE, Y_COORDINATE) {
    recvSock.connect(ipv4, port);
    
    uint32_t seed = 0; std::size_t tmp=0;
    if (recvSock.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }

    std::cout << "New client connected: " << recvSock.getRemoteAddress() << " SEED:" << seed << std::endl;
    
    next->setSeed(seed);
    curBlock = nullptr;
    mtx.unlock();
}

Competitor::~Competitor() {
    delete curBlock; curBlock = nullptr;
}

void Competitor::draw(sf::RenderWindow* window) {
    Monitor::draw(window);
    mtx.lock();
    map->drawCurrentBlock(window, curBlock, posY, posYShadow, posX);
    mtx.unlock();
}

void Competitor::start() {
    curBlock = next->updateNext();
    std::thread th([this](){
        while (true) {
            sf::Packet packet;
            if (recvSock.receive(packet) != sf::Socket::Done)
                throw std::runtime_error("Failed to receive event!");
    
            int messageCodeInt;
            packet >> messageCodeInt;

            switch (messageCodeInt) {
                case CURBLOCK: {
                    uint8_t state; uint8_t y, x, yShadow;
                    packet >> state >> y >> x >> yShadow;
                    mtx.lock();
                    curBlock->setStateID(state);
                    posX = x; posY = y; posYShadow = yShadow;
                    mtx.unlock();
                }
                break;

                case PUT: {
                    uint8_t state, y, x;
                    packet >> state >> y >> x;
                    mtx.lock();
                    map->update(curBlock, y, x);
                    delete curBlock; curBlock = next->updateNext();
                    mtx.unlock();
                }
                break;
                case HOLD: {
                    mtx.lock();
                    curBlock = hold->interchange(curBlock);
                    if (curBlock == nullptr) curBlock = next->updateNext();
                    mtx.unlock();
                }
                break;
                default:
                    break;
            }
            
            // mtx.lock();
            // pollEvent.push(event);
            // mtx.unlock();
        }
    });
    th.detach();
}