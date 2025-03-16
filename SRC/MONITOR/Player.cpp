#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

Player::Player(int X_COORDINATE, int Y_COORDINATE):Monitor(X_COORDINATE, Y_COORDINATE) {
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

#include <iostream>

Player::Player(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listener):Monitor(X_COORDINATE, Y_COORDINATE) {
    listener.accept(sendSock);
    std::cout << "New client connected: " << sendSock.getRemoteAddress() << std::endl;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

Player::Player(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port):Monitor(X_COORDINATE, Y_COORDINATE) {
    sendSock.connect(ipv4, port);
    std::cout << "New client connected: " << sendSock.getRemoteAddress() << std::endl;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

Player::~Player() {
}

void Player::start() {
    // next->draw(window); 
    // curBlock->draw(window, map);

    // while (window->isOpen()) {
        // processEvents();
        // render();
        // update();
    // }
}

void Player::sendEvent(const sf::Event &event) {
    // int byte = send(sendSock, (const char*)event, sizeof(event), 0);
}