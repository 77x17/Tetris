#include "Competitor.hpp"

#include "Player.hpp"

#include "Monitor_Multiplayer.hpp"
#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "CurrentBlock.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"
#include "MessageCode.hpp"
#include "Player_Multiplayer.hpp"

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed) {
    listenner.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    monitor = new Monitor_Multiplayer(); monitor->createMonitor(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlock();
    soundManager = new SoundManager();
    soundManager->loadSound("spin", "ASSETS/sfx/spin.mp3");
    mtx.unlock();
    monitor->setNewSeed(seed);
    socket.send(&seed, sizeof(seed));
}

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, char const* ipv4, int port){
    socket.connect(ipv4, port);
    uint32_t seed = 0; std::size_t tmp=0;
    if (socket.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    
    monitor = new Monitor_Multiplayer(); monitor->createMonitor(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlock();
    soundManager = new SoundManager();

    monitor->setNewSeed(seed);
    soundManager->loadSound("spin", "ASSETS/sfx/spin.mp3");
    mtx.unlock();
}

Competitor::~Competitor() {
    if (play.joinable()) play.join();
    delete curBlock; curBlock = nullptr;
    delete monitor;
    delete soundManager;
}

void Competitor::draw(sf::RenderWindow* window) {
    mtx.lock();
    monitor->draw(window, curBlock);
    mtx.unlock();
}

void Competitor::playing(Player_Multiplayer* &player){
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
                uint8_t state, y, x, shadowPosY, spin;
                packet >> state >> y >> x >> shadowPosY >> spin;
                curBlock->setState(state, x, y, shadowPosY);
                int nLinesRemove = monitor->putIntoMap(curBlock);
                if (nLinesRemove == 0) {
                    int seed; packet >> seed;
                    dynamic_cast<Monitor_Multiplayer*>(monitor)->mapReceiveLineFromCompetitor(seed);
                }

                // AllClearBUG ở đây
                nLinesRemove = monitor->removeNLines(nLinesRemove, curBlock, false);

                if (nLinesRemove > 0) {
                    player->handleAddLine(nLinesRemove);
                }
                curBlock->setter(monitor->getNext());
                monitor->unlockHold();
            }
            break;

            case RECVLINE: {
                uint8_t nLines;
                packet >> nLines;
                dynamic_cast<Monitor_Multiplayer*>(monitor)->inforReceiveLineFromCompetitor(nLines);
            }
            break;

            case HOLD: {
                mtx.lock();
                if (monitor->canHold()) {
                    monitor->exchangeCurrentBlock(curBlock);
                }
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

            case QUITGAME: {
                sf::Packet packet; packet << QUITGAME;
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
}

void Competitor::start(Player_Multiplayer* &player) { // Player
    if (play.joinable()) play.join();
    curBlock->freeAndSetter(monitor->getNext());
    play = std::thread(&Competitor::playing, this, std::ref(player));
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