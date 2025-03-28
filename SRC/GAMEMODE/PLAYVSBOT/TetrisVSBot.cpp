#include "TetrisVsBot.hpp"

#include "Common.hpp"
#include "PlayerWithBot.hpp"
#include "Bot.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include <iostream>

TetrisVsBot::TetrisVsBot(sf::RenderWindow* win, Scene* s):TetrisBaseMode(win, s) {
    std::random_device rd;
    gen = std::mt19937(rd());
    int PLAYER_X_COORDINATE = window->getSize().x / 4 - BLOCK_SIZE * 23 / 2;
    int PLAYER_Y_COORDINATE = 10;
    int COMPETITOR_X_COORDINATE = 3 * window->getSize().x / 4 - BLOCK_SIZE * 23 / 2;
    int COMPETITOR_Y_COORDINATE = 10;
    int tmp = gen();
    competitor = new Bot(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, tmp);
    player = new PlayerWithBot(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, tmp);
    player->setCompetitor(competitor);
}

TetrisVsBot::~TetrisVsBot() {
    delete player; player = nullptr; 
    delete competitor; competitor = nullptr;
}

STATUS_CODE TetrisVsBot::start() {
    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

    competitor->start(player);

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
                competitor->resetComponent();
                competitor->start(player);
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