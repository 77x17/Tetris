#include "Competitor.hpp"

#include "Player.hpp"

#include "MonitorForTwoPlayer.hpp"
#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "CurrentBlock.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"
#include "MessageCode.hpp"
#include "PlayerWithNetwork.hpp"

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <thread>

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed) {
    listenner.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    monitor = new MonitorForTwoPlayer(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlock();
    soundManager = new SoundManager();
    soundManager->loadSound("spin", "ASSETS/sfx/spin.mp3");
    mtx.unlock();
    monitor->setNewSeed(seed);
    socket.send(&seed, sizeof(seed));
}

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port){
    socket.connect(ipv4, port);
    uint32_t seed = 0; std::size_t tmp=0;
    if (socket.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    
    monitor = new MonitorForTwoPlayer(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlock();
    soundManager = new SoundManager();
    
    monitor->setNewSeed(seed);
    soundManager->loadSound("spin", "ASSETS/sfx/spin.mp3");
    mtx.unlock();
}

Competitor::~Competitor() {
    delete curBlock; curBlock = nullptr;
    delete monitor;
    delete soundManager;
}

void Competitor::draw(sf::RenderWindow* window) {
    mtx.lock();
    monitor->draw(window, curBlock);
    mtx.unlock();
}

void Competitor::start(PlayerWithNetwork* &player) { // Player
    curBlock->freeAndSetter((monitor->getNext())->updateNext());
    std::thread th([this](PlayerWithNetwork* &player){
        while (!monitor->isGameOver()) {
            sf::Packet packet;
            if (socket.receive(packet) != sf::Socket::Done)
                throw std::runtime_error("Failed to receive event! FROM competitor handler process");
    
            int messageCodeInt;
            packet >> messageCodeInt;

            switch (messageCodeInt) {
                case CURBLOCK: {
                    uint8_t state; uint8_t y, x, shadowPosY;
                    packet >> state >> y >> x >> shadowPosY;
                    mtx.lock();
                    curBlock->setState(state, x, y, shadowPosY);
                    mtx.unlock();
                }
                break;

                case PUT: {
                    uint8_t state, y, x, shadowPosY, spin, typeBlock;
                    packet >> state >> y >> x >> shadowPosY >> spin >> typeBlock;
                    curBlock->setState(state, x, y, shadowPosY);
                    int nLinesRemove = curBlock->putIntoMap(monitor->getMap());

                    if (nLinesRemove == 0) {
                        int seed; packet >> seed;
                        (monitor->getMap())->add((monitor->getInfor())->getAndRemoveLineAdd(), seed);
                    }

                    (monitor->getInfor())->update(nLinesRemove, spin, (char)typeBlock);
                    (monitor->getInfor())->playSound(nLinesRemove, spin, (char)typeBlock);
                    nLinesRemove = (monitor->getInfor())->removeLine(nLinesRemove);

                    if (nLinesRemove > 0) {
                        player->handleAddLine(nLinesRemove);
                    }
                    curBlock->setter((monitor->getNext())->updateNext());
                    (monitor->getHold())->unlock();
                }
                break;

                case RECVLINE: {
                    uint8_t nLines;
                    packet >> nLines;
                    (monitor->getInfor())->addLine(nLines);
                }
                break;

                case HOLD: {
                    mtx.lock();
                    if ((monitor->getHold())->canHold()) {
                        curBlock->swap(monitor->getHold());
                    }
                    (monitor->getHold())->lock();
                    if (curBlock->isEmpty())
                        curBlock->setter((monitor->getNext())->updateNext());
                    mtx.unlock();
                }
                break;

                case SPIN: {
                    soundManager->play("spin");
                }
                break;
                case GAMEOVER: {
                    sf::Packet packet; packet << GAMEOVER;
                    if (socket.send(packet) != sf::Socket::Done)
                        throw std::runtime_error("Failed to send event!");
                    monitor->setGameOver();
                }
                break;
                default: {
                    throw std::runtime_error("Error: Invalid value encountered - " + std::to_string(messageCodeInt));
                }
                break;
            }
        }
        std::cout << "END TURN!\n";
    }, std::ref(player));
    th.detach();
}

bool Competitor::isGameOver() {
    return monitor->isGameOver();
}

void Competitor::ready(int &seed) {
    int code = -1;
    sf::Packet packet;
    if (socket.receive(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to receive event from Ready signal!");
    packet >> code;
    if (code != RESTART) 
        throw std::runtime_error("Error: RESTART MESSAGE WRONG! code: " + std::to_string(code));
    packet >> seed;
    monitor->resetMonitor(seed);
}