#include "CurrentBlockWithNetwork.hpp"

#include "CurrentBlock.hpp"
#include "Block.hpp"

CurrentBlockWithNetwork::CurrentBlockWithNetwork():CurrentBlock(){}
CurrentBlockWithNetwork::CurrentBlockWithNetwork(Block *p):CurrentBlock(p){}

CurrentBlockWithNetwork::~CurrentBlockWithNetwork() {}

void CurrentBlockWithNetwork::compress(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY;
}

void CurrentBlockWithNetwork::compressWithSpin(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY << isJustSpin() << block->getStateID();
}