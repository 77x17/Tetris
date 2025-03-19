#include "Competitor.hpp"

#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "Block.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"
#include "PlayerWithNetwork.hpp"

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <thread>

// Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE):Monitor(X_COORDINATE, Y_COORDINATE) {
//     curBlock->setter(next->updateNext());
//     curBlock->resetPosition(map);
// }

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed) : Monitor(X_COORDINATE, Y_COORDINATE) {
    listenner.accept(recvSock);
    std::cout << "New client connected: " << recvSock.getRemoteAddress() << " SEED:" << seed << std::endl;

    next->setSeed(seed);
    recvSock.send(&seed, sizeof(seed));
    curBlock = nullptr;

    soundManager = new SoundManager();
    soundManager->loadSound("spin", "ASSETS/sfx/spin.mp3");

    mtx.unlock();
}

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port) : Monitor(X_COORDINATE, Y_COORDINATE) {
    recvSock.connect(ipv4, port);
    
    uint32_t seed = 0; std::size_t tmp=0;
    if (recvSock.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }

    std::cout << "New client connected: " << recvSock.getRemoteAddress() << " SEED:" << seed << std::endl;
    
    next->setSeed(seed);
    curBlock = nullptr;

    soundManager = new SoundManager();
    soundManager->loadSound("spin", "ASSETS/sfx/spin.mp3");

    mtx.unlock();
}

Competitor::~Competitor() {
    delete curBlock; curBlock = nullptr;

    delete soundManager;
}

void Competitor::draw(sf::RenderWindow* window) {
    Monitor::draw(window);
    mtx.lock();
    map->drawCurrentBlock(window, curBlock, posY, posYShadow, posX);
    mtx.unlock();
}

void Competitor::start(PlayerWithNetwork* &player) { // Player
    curBlock = next->updateNext();
    std::thread th([this](PlayerWithNetwork* &player){
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
                    uint8_t state, y, x, shadowPosY, spin, typeBlock;
                    packet >> state >> y >> x >> shadowPosY >> spin >> typeBlock;
                    mtx.lock();
                    int tmp = map->update(curBlock, y, x); 
                    hold->unlock();
                    delete curBlock; curBlock = next->updateNext();
                    
                    infor->removeLine(tmp);
                    infor->playSoundRemoveLine(tmp, spin, (char)typeBlock);
                    
                    if (tmp) {
                        
                        player->handleAddLine(tmp);
                    }
                    else {
                        map->add(infor->getAndRemoveLineAdd());
                    }

                    mtx.unlock();
                }
                break;

                case HOLD: {
                    mtx.lock();
                    curBlock = hold->interchange(curBlock);
                    hold->lock();
                    if (curBlock == nullptr) curBlock = next->updateNext();
                    mtx.unlock();
                }
                break;

                case SPIN: {
                    soundManager->play("spin");
                }
                break;

                default: {
                    
                }
                break;
            }
            
            // mtx.lock();
            // pollEvent.push(event);
            // mtx.unlock();
        }
    }, std::ref(player));
    th.detach();
}

void Competitor::handleAddLine(uint8_t nLines) {
    infor->addLine(nLines);
}