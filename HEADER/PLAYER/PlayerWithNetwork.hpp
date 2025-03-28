#ifndef PLAYERWITHNETWORK_HPP
#define PLAYERWITHNETWORK_HPP

#include <SFML/Window/Event.hpp>

#include "Player.hpp"

class Infor;
class MonitorForTwoPlayer;
class MovementControllerWithNetwork;

class PlayerWithNetwork: public Player{
private:
    MonitorForTwoPlayer* monitor;
    sf::TcpSocket socket;
    
public:
    PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    PlayerWithNetwork(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~PlayerWithNetwork();

    void initialize();

    void setGameOver();
    bool isGameOver();

    void draw(sf::RenderWindow* window);

    void start(uint32_t seed);
    void restart(uint32_t seed);

    void sendCurBlock();

    void handleAddLine(uint8_t nLines);

    void ready(int& seed);

    void waitingComfirm();
};

#endif