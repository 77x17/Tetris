#pragma once

#include <SFML/Graphics.hpp>

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
    int drawGameOver(sf::RenderWindow *window, sf::Sprite backgroundSprite, Player *player);
};
