#include "Menu.hpp"

#include "Player.hpp"

#include <iostream>

const int WINDOW_WIDTH  = 1050;
const int WINDOW_HEIGHT = 700;

const sf::Color MENU_BG_COLOR  = sf::Color(30, 30, 30);
const sf::Color TEXT_COLOR     = sf::Color::White;
const sf::Color SELECTED_COLOR = sf::Color::Yellow;

constexpr float TIME_OUT = 2.0f;

Menu::Menu() : mouseSelect(false) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

Menu::~Menu() {}

int Menu::createWindow(sf::RenderWindow *&window) {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris", sf::Style::Close);

    std::string menuItems[] = {"Single Player", "Multiple Player (Server)", "Multiple Player (Client)", "Quit"};
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

    bool choose = false;
    sf::Vector2i mousePosPrev = sf::Vector2i();
    while (window->isOpen() and not choose) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                selectedItem = menuSize - 1;
                
                choose = true;
                break;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedItem = (selectedItem - 1 + menuSize) % menuSize;
                    mouseSelect  = false;
                } else if (event.key.code == sf::Keyboard::Down) {
                    selectedItem = (selectedItem + 1           ) % menuSize;
                    mouseSelect  = false;
                } else if (event.key.code == sf::Keyboard::Space) {
                    choose = true;
                    break;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Lấy vị trí con trỏ trong cửa sổ
                    for (int i = 0; i < menuSize; i++) {
                        if (menuText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            selectedItem = i;

                            choose = true;
                            break;
                        }
                    }
                }
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Lấy vị trí con trỏ trong cửa sổ
        if (mousePos != mousePosPrev) {
            mouseSelect = true;
        }

        mousePosPrev = mousePos;

        if (mouseSelect) {
            for (int i = 0; i < menuSize; i++) {
                if (menuText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    selectedItem = i;
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

int Menu::waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish) {
    sf::Clock waitingClock;
    uint8_t count = 0;
    std::string waiting = "Waiting for another player"; 
    sf::Text waitingText(waiting, font, 40);
    waitingText.setPosition(WINDOW_WIDTH  / 2 - waitingText.getGlobalBounds().width / 2, 
                            WINDOW_HEIGHT / 2 - waitingText.getGlobalBounds().height / 2);

    while (window->isOpen() && !isFinish) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                
                return -1;
            }
        }

        window->clear(sf::Color(30, 30, 30));
        if (waitingClock.getElapsedTime().asSeconds() >= 0.5f) {
            count++;

            if (count == 4) {
                count = 0;
                waiting.erase(waiting.end() - 6, waiting.end());

                waitingText.setString(waiting);
            } 
            else {
                waitingText.setString(waiting += " .");
            }

            waitingClock.restart();
        }
        window->draw(waitingText);
        window->display();
    }
    return 0;
}

int Menu::drawGameOver(sf::RenderWindow *window, sf::Texture screenshot) {
    sf::Sprite background;                  // previous screen
    background.setTexture(screenshot);
    
    overlayTimeout.restart();

    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

    sf::Text gameOver("GAME OVER", font, 50);
    gameOver.setPosition(window->getSize().x / 2 - gameOver.getGlobalBounds().width / 2, 100);
    
    std::string menuItems[] = {"Restart", "Menu", "Quit"};
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    int selectedItem = 0;
    sf::Text menuText[menuSize];

    for (int i = 0; i < menuSize; i++) {
        menuText[i].setFont(font);
        menuText[i].setString(menuItems[i]);
        menuText[i].setCharacterSize(40);
        menuText[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
        menuText[i].setPosition(window->getSize().x / 2 - menuText[i].getGlobalBounds().width / 2, 
                                200 + i * 60);
    }

    bool choose = false;
    bool canPress = false;
    sf::Vector2i mousePosPrev = sf::Vector2i();
    while (window->isOpen() and not choose) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                selectedItem = menuSize - 1;
                
                choose = true;
                break;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedItem = (selectedItem - 1 + menuSize) % menuSize;
                    mouseSelect  = false;
                    
                    canPress     = true;
                } else if (event.key.code == sf::Keyboard::Down) {
                    selectedItem = (selectedItem + 1           ) % menuSize;
                    mouseSelect  = false;

                    canPress     = true;
                } else if (event.key.code == sf::Keyboard::Space and canPress) {
                    if (selectedItem == menuSize - 1) {
                        window->clear(MENU_BG_COLOR);
                    }
                    
                    choose = true;
                    break;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Lấy vị trí con trỏ trong cửa sổ
                    for (int i = 0; i < menuSize; i++) {
                        if (menuText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            selectedItem = i;

                            choose = true;
                            break;
                        }
                    }
                }
            }
        }

        if (overlayTimeout.getElapsedTime().asSeconds() > TIME_OUT) {
            canPress = true;
        }

        window->clear();

        window->draw(background); // Draw background

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f))); 
        window->draw(overlay);
        
        window->draw(gameOver);        
        
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Lấy vị trí con trỏ trong cửa sổ
        if (mousePos != mousePosPrev) {
            mouseSelect = true;
        }
        
        mousePosPrev = mousePos;

        if (mouseSelect) {
            for (int i = 0; i < menuSize; i++) {
                if (menuText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    selectedItem = i;
                }
            }
        }
        for (int i = 0; i < menuSize; i++) {
            menuText[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
        }

        for (int i = 0; i < menuSize; i++) {
            window->draw(menuText[i]);
        }
        window->display();
    }
    
    return (selectedItem == menuSize - 1 ? -1 : selectedItem);
}