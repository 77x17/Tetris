#pragma once

#include <SFML/Graphics.hpp>

class Menu {
private:
    sf::Font font;

public:
    Menu();
    ~Menu();

    int createWindow(sf::RenderWindow *window);

};