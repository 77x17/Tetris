#ifndef CURRENTBLOCKWITHNETWORK_HPP
#define CURRENTBLOCKWITHNETWORK_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "CurrentBlock.hpp"

class Map;
class Block;
class Hold;

class CurrentBlockWithNetwork: public CurrentBlock {
public:
    CurrentBlockWithNetwork();
    CurrentBlockWithNetwork(Block *p);
    ~CurrentBlockWithNetwork();

    void compress(sf::Packet &packet);
    void compressWithSpin(sf::Packet &packet);
};

#endif