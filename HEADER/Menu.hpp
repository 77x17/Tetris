#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

enum class STATUS_CODE {QUIT = -1, SINGLEPLAYER = 0, VERSUSBOT = 1, MULTIPLAYER_SERVER = 2, MULTIPLAYER_CLIENT = 3, RESTART = 0, MENU = 1};

class Player;

class Menu {
private:
    sf::Font  font;
    
    sf::Clock overlayTimeout;
    bool      mouseSelect;

public:
    Menu();
    ~Menu();

    STATUS_CODE drawMenu(sf::RenderWindow *window);
    int waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish);
    STATUS_CODE drawGameOver(sf::RenderWindow *window, sf::Texture screenshot);
};
