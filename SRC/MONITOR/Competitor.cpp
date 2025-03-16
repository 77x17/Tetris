#include "Competitor.hpp"

#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

Competitor::Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpSocket *socket):Monitor(X_COORDINATE, Y_COORDINATE) {
    recvSock = socket;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

Competitor::~Competitor() {
    if (recvSock == nullptr)
        delete recvSock;
}

void Competitor::start() {
    autoDown();
}