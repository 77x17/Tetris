#include "Tetris_VersusBot.hpp"

#include "Common.hpp"
#include "Player_VersusBot.hpp"
#include "Bot.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include <iostream>

#include "BotBrain.hpp"
int64_t BotBrain::sumScore = 0;
int8_t BotBrain::countSet = 0;

Tetris_VersusBot::Tetris_VersusBot(sf::RenderWindow* win, Scene* s):Tetris(win, s) {
    PLAYER_X_COORDINATE = window->getSize().x / 4 - Common::BLOCK_SIZE * 23 / 2;
    PLAYER_Y_COORDINATE = 10;
    COMPETITOR_X_COORDINATE = 3 * window->getSize().x / 4 - Common::BLOCK_SIZE * 23 / 2;
    COMPETITOR_Y_COORDINATE = 10;
    player = new Player_VersusBot(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE);
    competitor = new Bot(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE);
    std::random_device rd;
    gen = std::mt19937(rd());
}

Tetris_VersusBot::~Tetris_VersusBot() {
    delete player; player = nullptr; 
    delete competitor; competitor = nullptr;
}

STATUS_CODE Tetris_VersusBot::start() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

restartVersusBot:
    int tmp = gen();
    player->start(tmp, competitor);
    competitor->start(tmp, player);
    
    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    // Fade in: change menu
    {
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        competitor->draw(window);
        window->display();
        
        scene->drawChangeMenu(window, true);
    }

    // Countdown:
    {
        int HOLD_WIDTH         = 5;
        
        int GRID_WIDTH         = 10;
        int GRID_HEIGHT        = 24;
        int GRID_POSITION_X    = PLAYER_X_COORDINATE + HOLD_WIDTH * Common::BLOCK_SIZE + Common::BLOCK_SIZE + Common::BLOCK_SIZE;
        int GRID_POSITION_Y    = PLAYER_Y_COORDINATE;
        int GRID_COMPETITOR_POSITION_X    = COMPETITOR_X_COORDINATE + HOLD_WIDTH * Common::BLOCK_SIZE + Common::BLOCK_SIZE + Common::BLOCK_SIZE;
        int GRID_COMPETITOR_POSITION_Y    = COMPETITOR_Y_COORDINATE;
        
        scene->drawCountdown(window, 
            GRID_POSITION_X + GRID_WIDTH  * Common::BLOCK_SIZE / 2 - Common::WIDTH_BORDER,
            GRID_POSITION_Y + GRID_HEIGHT * Common::BLOCK_SIZE / 2 - Common::WIDTH_BORDER,
            GRID_COMPETITOR_POSITION_X + GRID_WIDTH  * Common::BLOCK_SIZE / 2 - Common::WIDTH_BORDER,
            GRID_COMPETITOR_POSITION_Y + GRID_HEIGHT * Common::BLOCK_SIZE / 2 - Common::WIDTH_BORDER
        );
    }

    player->setTimer();
    competitor->setTimer();

    while (not (player->isGameOver() or competitor->isGameOver())) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);

                goto quitVersusBot;
            }
            else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
                player->pauseTimer();
                competitor->pauseTimer();

                STATUS_CODE escapeOption = scene->drawPause(window);

                switch (escapeOption) {
                    case STATUS_CODE::RESUME:
                        player->unPauseTimer();
                        competitor->unPauseTimer();
                        
                        break;
                    case STATUS_CODE::RESTART:
                        screenStatus = STATUS_CODE::RESTART;
                        
                        goto quitVersusBot;
                    case STATUS_CODE::MENU:
                        screenStatus = STATUS_CODE::MENU;
                        
                        goto quitVersusBot;
                    case STATUS_CODE::QUIT:
                        screenStatus = STATUS_CODE::QUIT;
                        
                        goto quitVersusBot;
                    default:
                        throw std::runtime_error("[escapeOption] cannot find STATUS_CODE");
                }
            }

            player->processEvents(event);
        }

        player->autoDown();
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        competitor->draw(window);
        window->display();

        if (competitor->isGameOver()) {
            player->setGameOver();
            
            window->clear();
            window->display();

            screenStatus = scene->drawVictory(window);
        }

        if (player->isGameOver()) { // Game over
            competitor->setGameOver();

            window->clear();
            window->display();

            screenStatus = scene->drawGameOver(window);
        }
    }

quitVersusBot:
    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartVersusBot;
    }

    return screenStatus;
}