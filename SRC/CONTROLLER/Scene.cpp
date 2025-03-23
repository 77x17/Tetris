#include "Scene.hpp"

#include "Menu.hpp"
#include "SoundManager.hpp"
#include "Common.hpp"

#include <iostream>

const int TITLE_PADDING   = 100;

constexpr float TIME_OUT = 1.0f;

Scene::Scene(sf::RenderWindow *window) : mouseSelect(false) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
    
    soundManager = new SoundManager();
    soundManager->loadSound("countdown", "ASSETS/sfx/countdown.mp3");

    menuBackgroundTexture.loadFromFile("ASSETS/menuBackground.png");
    menuBackground.setTexture(menuBackgroundTexture);
    menuBackground.setColor(sf::Color(255, 255, 255, 50));

    // Get window size & texture size
    sf::Vector2u windowSize  = window->getSize();
    sf::Vector2u textureSize = menuBackgroundTexture.getSize();
    // Calculate scale factors
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::max(scaleX, scaleY);
    // Apply scale to fit window
    menuBackground.setScale(scale, scale);
    // Center the sprite
    float newWidth  = textureSize.x * scale;
    float newHeight = textureSize.y * scale;
    float posX = (windowSize.x - newWidth ) / 2;
    float posY = (windowSize.y - newHeight) / 2;
    menuBackground.setPosition(posX, posY);

    mainMenu = new Menu(window, {
        "Single Player",
        "Tetr.io with Bot",
        "Multiple Player (Server)",
        "Multiple Player (Client)",
        "Quit"
    }, MENU_CODE::MAIN);

    pauseMenu = new Menu(window, {
        "Restart",
        "Menu",
        "Resume",
        "Quit"
    }, MENU_CODE::PAUSE);

    gameOverMenu = new Menu(window, {
        "Restart",
        "Menu",
        "Quit"
    }, MENU_CODE::GAMEOVER);
}

Scene::~Scene() {
    delete soundManager;

    delete mainMenu;
    delete pauseMenu;
    delete gameOverMenu;
}

void Scene::drawChangeMenu(sf::RenderWindow *window, bool fadeIn) {
    sf::Texture screenshot;
    sf::Sprite  background;
    screenshot.create(window->getSize().x, window->getSize().y);
    
    window->display();

    screenshot.update(*window);
    background.setTexture(screenshot);
    
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));

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

STATUS_CODE Scene::drawMenu(sf::RenderWindow *window) {
    {
        window->clear();

        window->draw(menuBackground);

        mainMenu->draw(window);

        window->display();

        drawChangeMenu(window, true);
    }

    while (window->isOpen() and mainMenu->notSelected()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            mainMenu->processEvents(window, event);
        }

        mainMenu->update(window);

        window->clear();

        window->draw(menuBackground);

        mainMenu->draw(window);

        window->display();
    }

    drawChangeMenu(window, false);

    return mainMenu->getSelectedItem();
}

int Scene::waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish) {
    sf::Clock waitingClock;
    uint8_t count = 0;
    std::string waiting = "Waiting for another player"; 
    sf::Text waitingText(waiting, font, 40);
    waitingText.setPosition(window->getSize().x  / 2 - waitingText.getGlobalBounds().width / 2, 
                            window->getSize().y / 2 - waitingText.getGlobalBounds().height / 2);

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

STATUS_CODE Scene::drawGameOver(sf::RenderWindow *window) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);
    sf::Sprite background;
    background.setTexture(screenshot);

    overlayTimeout.restart();
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));

    sf::Text titleText("GAME OVER", font, 50);
    titleText.setPosition(window->getSize().x / 2 - titleText.getGlobalBounds().width / 2, TITLE_PADDING);
    
    while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
        sf::Event event;
        while (window->pollEvent(event)) {
            // Chờ để hủy Restart ngay lập tức
        }

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f)));        // 200.0f để mờ mờ nhìn thấy background
        
        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        window->draw(titleText);    
        window->display();
    }

    while (window->isOpen() and gameOverMenu->notSelected()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            gameOverMenu->processEvents(window, event);
        }

        gameOverMenu->update(window);

        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        window->draw(titleText);        
        gameOverMenu->draw(window);
        window->display();
    }
    
    drawChangeMenu(window, false);

    return gameOverMenu->getSelectedItem();
}

STATUS_CODE Scene::drawPause(sf::RenderWindow *window) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);
    sf::Sprite background;
    background.setTexture(screenshot);
    
    overlayTimeout.restart();
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));

    sf::Text titleText("PAUSE", font, 50);
    titleText.setPosition(window->getSize().x / 2 - titleText.getGlobalBounds().width / 2, TITLE_PADDING);

    while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
        sf::Event event;
        while (window->pollEvent(event)) {
            // Chờ để hủy Restart ngay lập tức
        }

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f)));        // 200.0f để mờ mờ nhìn thấy background
        
        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        window->draw(titleText);    
        window->display();
    }

    while (window->isOpen() and pauseMenu->notSelected()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            pauseMenu->processEvents(window, event);
        }

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(128, 128, 128, std::min(alpha, 200.0f))); 

        pauseMenu->update(window);

        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        window->draw(titleText);
        pauseMenu->draw(window);
        window->display();
    }

    STATUS_CODE result = pauseMenu->getSelectedItem();

    if (result == STATUS_CODE::RESUME) {
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

    return result;
}

void Scene::drawCountdown(sf::RenderWindow *window, int gridCenterX, int gridCenterY) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);
    sf::Sprite background;
    background.setTexture(screenshot);

    sf::Clock timeout;
    sf::Clock effectTimeout;
    
    soundManager->play("countdown");

    int count = 3;
    while (count >= 0) {
        sf::Event event;
        while (window->pollEvent(event)) {
            // clear buffer
        }

        if (timeout.getElapsedTime().asSeconds() > 1) {
            count--;

            timeout.restart();
            effectTimeout.restart();
        }

        float alpha = 255 * (1 - effectTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        float scaleFactor = 1.0f + 1.0f * effectTimeout.getElapsedTime().asSeconds();;  // Tăng scale

        sf::Text countdown(count ? std::to_string(count) : "GO!", font, 40);

        countdown.setFillColor(sf::Color(255, 255, 0, alpha)); // Vàng nhưng giảm alpha
        countdown.setScale(scaleFactor, scaleFactor);
        countdown.setPosition(sf::Vector2f(
            gridCenterX - countdown.getGlobalBounds().width / 2,
            gridCenterY - countdown.getGlobalBounds().height / 2
        ));

        window->clear();
        window->draw(background);
        window->draw(countdown);
        window->display();
    }
}