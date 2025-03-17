#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>

#include <atomic>

class Player;
class PlayerWithNetwork;
class Competitor;
class NetworkManager;

class Tetris {
private:
    sf::RenderWindow *window;

    std::atomic<bool> isFinish;
    void makeConnection(bool isHost, Competitor* &competitor,PlayerWithNetwork* &player);

public:
    Tetris();
    ~Tetris();
    void startGameOnePlayer();
    void startGameTwoPlayer(bool isHost);
};
#endif