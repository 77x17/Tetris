#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>

#include <atomic>

class Player;
class PlayerWithNetwork;
class Competitor;
class NetworkManager;
class Menu;

class Tetris {
private:
    sf::RenderWindow *window;

    Menu             *menu;

    std::atomic<bool> isFinish;
    void makeConnection(bool isHost, Competitor* &competitor,PlayerWithNetwork* &player);

public:
    Tetris();
    ~Tetris();
    void start();
    void startGameOnePlayer();
    void startGameTwoPlayer(bool isHost);
};
#endif