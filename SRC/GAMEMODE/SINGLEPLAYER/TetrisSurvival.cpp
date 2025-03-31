#include "TetrisSurvival.hpp"

#include "Common.hpp"
#include "PlayerSurvival.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include "SoundManager.hpp"

#include <iostream>

TetrisSurvival::TetrisSurvival(sf::RenderWindow* win, Scene* s):TetrisBaseMode(win, s) {
    X_COORDINATE = window->getSize().x / 2 - Common::BLOCK_SIZE * 23 / 2 - Common::BLOCK_SIZE;
    Y_COORDINATE = 10;
    player = new PlayerSurvival(X_COORDINATE, Y_COORDINATE);
}

TetrisSurvival::~TetrisSurvival() {
    delete player; player = nullptr;
}

STATUS_CODE TetrisSurvival::start() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

restartGameSurvival:
    player->start();

    STATUS_CODE screenStatus = STATUS_CODE::QUIT;
    
    // Skip
    // Fade in: change menu
    {
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();
        
        scene->drawChangeMenu(window, true);
    }
    
    // Countdown: Skip
    {
        int HOLD_WIDTH         = 5;
        
        int GRID_WIDTH         = 10;
        int GRID_HEIGHT        = 24;
        int GRID_POSITION_X    = X_COORDINATE + HOLD_WIDTH * Common::BLOCK_SIZE + Common::BLOCK_SIZE + Common::BLOCK_SIZE;
        int GRID_POSITION_Y    = Y_COORDINATE;
        
        scene->drawCountdown(window, 
            (GRID_POSITION_X + GRID_WIDTH  * Common::BLOCK_SIZE / 2 - Common::WIDTH_BORDER),
            (GRID_POSITION_Y + GRID_HEIGHT * Common::BLOCK_SIZE / 2 - Common::WIDTH_BORDER),
            -1,
            -1
        );
    }

    std::cout << "ok\n";

    player->setTimer();

    std::cout << "ok\n";

    sf::Event event;
    while (not player->isGameOver()) {
        while (window->pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);
                
                goto quitStartGameSurvival;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    player->pauseTimer();

                    STATUS_CODE escapeOption = scene->drawPause(window);

                    switch (escapeOption) {
                        case STATUS_CODE::RESUME:
                            player->unPauseTimer();
                            
                            break;
                        case STATUS_CODE::RESTART:
                            screenStatus = STATUS_CODE::RESTART;
                            
                            goto quitStartGameSurvival;
                        case STATUS_CODE::MENU:
                            screenStatus = STATUS_CODE::MENU;
                            
                            goto quitStartGameSurvival;
                        case STATUS_CODE::QUIT:
                            screenStatus = STATUS_CODE::QUIT;
                            
                            goto quitStartGameSurvival;
                        default:
                            throw std::runtime_error("[escapeOption] cannot find STATUS_CODE");
                    }
                }
            }

            player->processEvents(event);
        }

        player->autoDown();

        if (timer.getElapsedTime().asSeconds() >= 10) {
            
            // sent 4 lines
            player->putGarbage();
            player->receiveGarbage(4);
            
            timer.restart();
        }
        
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();

        if (player->isGameOver()) {
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            window->display();

            screenStatus = scene->drawGameOver(window);
        }
    }

quitStartGameSurvival:
    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartGameSurvival;
    }

    return screenStatus;
}