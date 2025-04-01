#include "Tetris_VersusBot.hpp"

#include "Common.hpp"
#include "Player_VersusBot.hpp"
#include "Bot.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include <iostream>

Tetris_VersusBot::Tetris_VersusBot(sf::RenderWindow* win, Scene* s):Tetris(win, s) {
    int PLAYER_X_COORDINATE = window->getSize().x / 4 - Common::BLOCK_SIZE * 23 / 2;
    int PLAYER_Y_COORDINATE = 10;
    int COMPETITOR_X_COORDINATE = 3 * window->getSize().x / 4 - Common::BLOCK_SIZE * 23 / 2;
    int COMPETITOR_Y_COORDINATE = 10;
    competitor = new Bot(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE);
    player = new Player_VersusBot(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE);
    std::random_device rd;
    gen = std::mt19937(rd());
}

Tetris_VersusBot::~Tetris_VersusBot() {
    delete player; player = nullptr; 
    delete competitor; competitor = nullptr;
}

STATUS_CODE Tetris_VersusBot::start() {
    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

    int tmp = gen();
    player->start(tmp, competitor);
    competitor->start(tmp, player);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            player->processEvents(event);
        }
        if (competitor->isGameOver()) {
            player->setGameOver();
        }
        if (!player->isGameOver()) {
            player->autoDown();
            competitor->update();
            
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            competitor->draw(window);
            window->display();
        }
        else {
            STATUS_CODE option = scene->drawGameOver(window);
            if (option == STATUS_CODE::RESTART) {
                int tmp = gen();
                player->start(tmp, competitor);
                competitor->start(tmp, player);
            }
            else if (option == STATUS_CODE::MENU) {     // Menu
                window->close();
            }
            else if (option == STATUS_CODE::QUIT) {    // Quit
                window->close();
            }
        }
    }

    return screenStatus;
}