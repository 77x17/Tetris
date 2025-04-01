#ifndef CURRENTBLOCK_MULTIPLAYER_HPP
#define CURRENTBLOCK_MULTIPLAYER_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "CurrentBlock.hpp"

class Map;
class Block;
class Hold;

class CurrentBlock_Multiplayer: public CurrentBlock {
public:
    CurrentBlock_Multiplayer();
    CurrentBlock_Multiplayer(Block *p);
    ~CurrentBlock_Multiplayer();

    void compress(sf::Packet &packet);
    void compressWithSpin(sf::Packet &packet);
};

#endif