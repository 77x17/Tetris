#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <atomic>

class Player;
class PlayerWithNetwork;
class Competitor;
class NetworkManager;
class Menu;

enum class STATUS_CODE;

class Tetris {
private:
    Menu             *menu;
    sf::RenderWindow *window;

    sf::Font          font;

    std::atomic<bool> isFinish;
    void makeConnection(bool isHost, Competitor* &competitor, PlayerWithNetwork* &player);

public:
    Tetris();
    ~Tetris();
    void start();
    void loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite, sf::Music &backgroundMusic);
    STATUS_CODE startGameVersusBot();
    STATUS_CODE startGameOnePlayer();
    void startGameTwoPlayer(bool isHost);
};

#endif