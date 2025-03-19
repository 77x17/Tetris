#include "Menu.hpp"

#include <iostream>

const int WINDOW_WIDTH  = 1050;
const int WINDOW_HEIGHT = 700;

const sf::Color MENU_BG_COLOR  = sf::Color(30, 30, 30);
const sf::Color TEXT_COLOR     = sf::Color::White;
const sf::Color SELECTED_COLOR = sf::Color::Yellow;

Menu::Menu() {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

Menu::~Menu() {}

int Menu::createWindow(sf::RenderWindow *window) {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris", sf::Style::Close);
    std::string menuItems[] = {"Single Player", "Multiple Player (Server)", "Multiple Player (Client)", "Exit"};
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedItem = 0;
    sf::Text menuText[menuSize];

    for (int i = 0; i < menuSize; i++) {
        menuText[i].setFont(font);
        menuText[i].setString(menuItems[i]);
        menuText[i].setCharacterSize(40);
        menuText[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
        menuText[i].setPosition(window->getSize().x / 2 - menuText[i].getGlobalBounds().width / 2, 
                                50 + i * 60);
    }

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                selectedItem = menuSize - 1;

                window->close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedItem = (selectedItem - 1 + menuSize) % menuSize;
                } else if (event.key.code == sf::Keyboard::Down) {
                    selectedItem = (selectedItem + 1           ) % menuSize;
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedItem == menuSize - 1) {
                        window->clear(MENU_BG_COLOR);
                    }

                    window->close();
                }
            }
        }

        for (int i = 0; i < menuSize; i++) {
            menuText[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
        }

        window->clear(MENU_BG_COLOR);
        for (int i = 0; i < menuSize; i++) {
            window->draw(menuText[i]);
        }
        window->display();
    }

    return (selectedItem == menuSize - 1 ? -1 : selectedItem);
}

