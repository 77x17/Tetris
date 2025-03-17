#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>

#include <atomic>

class Player;
class Competitor;
class NetworkManager;

class Tetris {
private:
    Player *player;
    Competitor *competitor;
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