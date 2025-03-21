#include "Menu.hpp"

#include "SoundManager.hpp"

#include <iostream>

const int WINDOW_WIDTH   = 1100;
const int WINDOW_HEIGHT  = 700;

const int BAR_PADDING      = 20;
const int OPTION_PADDING   = 100;
const int SELECTED_PADDING = 50;

const sf::Color MENU_BG_COLOR  = sf::Color(30, 30, 30);
const sf::Color MENU_BAR_COLOR = sf::Color(60, 60, 60);
const sf::Color TEXT_COLOR     = sf::Color::White;
const sf::Color SELECTED_COLOR = sf::Color::Yellow;

constexpr float TIME_OUT = 1.0f;

Menu::Menu() : mouseSelect(false) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
    
    soundManager = new SoundManager();
    soundManager->loadSound("move", "ASSETS/sfx/move.mp3");
}

Menu::~Menu() {
    delete soundManager;
}

void Menu::drawChangeMenu(sf::RenderWindow *window, bool fadeIn) {
    sf::Texture screenshot;
    sf::Sprite  background;
    screenshot.create(window->getSize().x, window->getSize().y);
    
    window->display();

    screenshot.update(*window);
    background.setTexture(screenshot);
    
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

    overlayTimeout.restart(); 
    while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
        sf::Event tempEvent;
        while (window->pollEvent(tempEvent)) {
            // Không làm gì cả -> Chỉ lấy ra để loại bỏ buffer
        }

        window->clear();

        window->draw(background); // Draw background

        if (fadeIn) {
            float alpha = 255 * (1 - overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
            overlay.setFillColor(sf::Color(128, 128, 128, alpha));
        }
        else {
            float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
            overlay.setFillColor(sf::Color(128, 128, 128, alpha)); 
        }
        window->draw(overlay);

        window->display();
    }
}

STATUS_CODE Menu::drawMenu(sf::RenderWindow *window) {
    std::string menuItems[] = {"Single Player", "Tetr.io with Bot", "Multiple Player (Server)", "Multiple Player (Client)", "Quit"};
    int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
    sf::RectangleShape menuBar[menuSize];
    sf::Text menuText[menuSize];
    int selectedItem = 0;
    for (int i = 0; i < menuSize; i++) {
        menuText[i].setFont(font);
        menuText[i].setString(menuItems[i]);
        menuText[i].setCharacterSize(40);
        menuText[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
        menuText[i].setPosition(window->getSize().x / 3, OPTION_PADDING + i * OPTION_PADDING);

        menuBar[i].setPosition(sf::Vector2f(window->getSize().x / 3 - BAR_PADDING, OPTION_PADDING + i * OPTION_PADDING));
        menuBar[i].setSize(sf::Vector2f(WINDOW_WIDTH - (window->getSize().x / 3 - BAR_PADDING) + SELECTED_PADDING, OPTION_PADDING - BAR_PADDING));
        menuBar[i].setFillColor(MENU_BAR_COLOR);
    }
    
    {
        window->clear(MENU_BG_COLOR);

        for (int i = 0; i < menuSize; i++) {
            window->draw(menuBar[i]);
            window->draw(menuText[i]);
        }

        window->display();

        drawChangeMenu(window, true);
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

                    soundManager->play("move");
                } else if (event.key.code == sf::Keyboard::Down) {
                    selectedItem = (selectedItem + 1           ) % menuSize;
                    mouseSelect  = false;

                    soundManager->play("move");
                } else if (event.key.code == sf::Keyboard::Space) {
                    choose = true;
                    break;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Lấy vị trí con trỏ trong cửa sổ
                    for (int i = 0; i < menuSize; i++) {
                        if (menuBar[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
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
                if (menuBar[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    if (selectedItem != i) {
                        selectedItem = i;
                        
                        soundManager->play("move");
                    }
                }
            }
        }

        for (int i = 0; i < menuSize; i++) {
            if (i == selectedItem) {
                menuText[i].setFillColor(SELECTED_COLOR);
                menuText[i].setPosition(sf::Vector2f(window->getSize().x / 3 - SELECTED_PADDING, OPTION_PADDING + i * OPTION_PADDING));

                menuBar[i].setPosition(sf::Vector2f(window->getSize().x / 3 - BAR_PADDING - SELECTED_PADDING, OPTION_PADDING + i * OPTION_PADDING));
            }
            else {
                menuText[i].setFillColor(TEXT_COLOR);
                menuText[i].setPosition(sf::Vector2f(window->getSize().x / 3, OPTION_PADDING + i * OPTION_PADDING));

                menuBar[i].setPosition(sf::Vector2f(window->getSize().x / 3 - BAR_PADDING, OPTION_PADDING + i * OPTION_PADDING));
            }
        }

        window->clear(MENU_BG_COLOR);

        for (int i = 0; i < menuSize; i++) {
            window->draw(menuBar[i]);
            window->draw(menuText[i]);
        }

        window->display();
    }

    drawChangeMenu(window, false);

    return static_cast<STATUS_CODE>(selectedItem == menuSize - 1 ? -1 : selectedItem);
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

STATUS_CODE Menu::drawGameOver(sf::RenderWindow *window) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);

    sf::Sprite background;
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

                    soundManager->play("move");
                } else if (event.key.code == sf::Keyboard::Down) {
                    selectedItem = (selectedItem + 1           ) % menuSize;
                    mouseSelect  = false;
                    
                    canPress     = true;

                    soundManager->play("move");
                } else if (event.key.code == sf::Keyboard::Space and canPress) {
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
        overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f)));        // 200.0f để mờ mờ nhìn thấy background
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
                    if (selectedItem != i) {
                        selectedItem = i;

                        soundManager->play("move");
                    }
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
    
    drawChangeMenu(window, false);

    return static_cast<STATUS_CODE>(selectedItem == menuSize - 1 ? -1 : selectedItem);
}

STATUS_CODE Menu::drawEscape(sf::RenderWindow *window) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);

    sf::Sprite background;
    background.setTexture(screenshot);
    
    overlayTimeout.restart();

    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
 
    std::string menuItems[] = {"Resume", "Menu", "Quit"};
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
                    
                    soundManager->play("move");
                } 
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedItem = (selectedItem + 1           ) % menuSize;
                    mouseSelect  = false;

                    soundManager->play("move");
                } 
                else if (event.key.code == sf::Keyboard::Space) {
                    choose = true;
                    
                    break;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    choose = true;
                    selectedItem = 0;

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

        window->clear();

        window->draw(background); // Draw background

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f))); 
        window->draw(overlay);
        
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Lấy vị trí con trỏ trong cửa sổ
        if (mousePos != mousePosPrev) {
            mouseSelect = true;
        }
        
        mousePosPrev = mousePos;

        if (mouseSelect) {
            for (int i = 0; i < menuSize; i++) {
                if (menuText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    if (selectedItem != i) {
                        selectedItem = i;
                        
                        soundManager->play("move");
                    }
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

    if (selectedItem == 0) {
        overlayTimeout.restart(); 
        while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
            sf::Event tempEvent;
            while (window->pollEvent(tempEvent)) {
                // Không làm gì cả -> Chỉ lấy ra để loại bỏ buffer
            }

            window->clear();

            window->draw(background); // Draw background
            
            float alpha = 255 * (1 - overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
            overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f)));
            
            window->draw(overlay);

            window->display();
        }
    }
    else {
        drawChangeMenu(window, false);
    }

    return static_cast<STATUS_CODE>(selectedItem == menuSize - 1 ? -1 : selectedItem);
}