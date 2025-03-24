#ifndef CURRENTBLOCKCONTROLLERWITHNETWORK_HPP
#define CURRENTBLOCKCONTROLLERWITHNETWORK_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "CurrentBlockController.hpp"

class CurrentBlock;
class Map;
class Infor;
class SoundManager;

class CurrentBlockControllerWithNetwork: public CurrentBlockController {
private:
    sf::TcpSocket socket;
    Infor* infor;
public:
    CurrentBlockControllerWithNetwork(Map*, Infor*);
    ~CurrentBlockControllerWithNetwork();

    bool rotateLeft();
    bool rotateRight();

    void swap(Hold* hold);

    uint8_t putIntoMap();

    void compress(sf::Packet &packet);
    void compressWithSpin(sf::Packet &packet);
};

#endif