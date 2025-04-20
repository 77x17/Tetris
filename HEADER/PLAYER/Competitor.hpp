#ifndef COMPETITOR_HPP
#define COMPETITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <mutex>
#include <thread>

class CurrentBlock;
class SoundManager;
class Player_Multiplayer;
class Monitor_Multiplayer;

class Competitor{
private:
    Monitor_Multiplayer* monitor;
    sf::TcpSocket socket;
    
    CurrentBlock* curBlock;
    SoundManager *soundManager;
    
    std::mutex mtx;
    std::thread play;

    void playing(Player_Multiplayer* &player);

public:
    // Competitor(int X_COORDINATE, int Y_COORDINATE);
    Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    Competitor(int X_COORDINATE, int Y_COORDINATE, char const* ipv4, int port);
    ~Competitor();
    void start(Player_Multiplayer* &player);
    void ready(int& seed);

    bool isGameOver();

    void draw(sf::RenderWindow* window);
};

#endif