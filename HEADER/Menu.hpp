#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

class Player;

class Menu {
private:
    sf::Font  font;
    
    sf::Clock overlayTimeout;
    bool      mouseSelect;

public:
    Menu();
    ~Menu();

    int createWindow(sf::RenderWindow *&window);
    int waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish);
    int drawGameOver(sf::RenderWindow *window, sf::Texture screenshot);
};
