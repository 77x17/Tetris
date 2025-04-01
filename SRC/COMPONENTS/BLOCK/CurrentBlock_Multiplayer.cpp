#include "CurrentBlock_Multiplayer.hpp"

#include "CurrentBlock.hpp"
#include "Block.hpp"

CurrentBlock_Multiplayer::CurrentBlock_Multiplayer():CurrentBlock(){}
CurrentBlock_Multiplayer::CurrentBlock_Multiplayer(Block *p):CurrentBlock(p){}

CurrentBlock_Multiplayer::~CurrentBlock_Multiplayer() {}


void CurrentBlock_Multiplayer::compress(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY;
}

void CurrentBlock_Multiplayer::compressWithSpin(sf::Packet &packet) {
    packet << block->getStateID() << posY << posX << shadowPosY << spin << (uint8_t)getTypeBlock();
}