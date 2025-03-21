#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

enum class STATUS_CODE {
    QUIT               = -1, 
    SINGLEPLAYER       = 0, 
    VERSUSBOT          = 1, 
    MULTIPLAYER_SERVER = 2, 
    MULTIPLAYER_CLIENT = 3, 
    RESTART            = 0, 
    MENU               = 1, 
    RESUME             = 0
};

class SoundManager;

class Menu {
private:
    sf::Font      font;
    SoundManager *soundManager;
    
    sf::Clock     overlayTimeout;
    bool          mouseSelect;

public:
    Menu();
    ~Menu();

    // fadeIn: mờ thành rõ (true) | Default* fadeOut: rõ thành mờ (false)
    void drawChangeMenu(sf::RenderWindow *window, bool fadeIn = false);

    STATUS_CODE drawMenu(sf::RenderWindow *window);
    STATUS_CODE drawEscape(sf::RenderWindow *window);
    int waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish);
    STATUS_CODE drawGameOver(sf::RenderWindow *window);
};
