#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>

#include <atomic>

class Monitor;
class NetworkManager;

class Tetris {
private:
    Monitor *player, *competitor;
    sf::RenderWindow *window;

    std::atomic<bool> isFinish;
    void makeConnection(bool isHost);

public:
    Tetris();
    ~Tetris();
    void startGameOnePlayer();
    void startGameTwoPlayer(bool isHost);
};
#endif