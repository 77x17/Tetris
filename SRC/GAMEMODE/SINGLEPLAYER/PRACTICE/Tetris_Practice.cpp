#include "Tetris_Practice.hpp"

#include "Common.hpp"
#include "Player.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include "SoundManager.hpp"

Tetris_Practice::Tetris_Practice(sf::RenderWindow* win, Scene* s):Tetris(win, s) {
    X_COORDINATE = window->getSize().x / 2 - Common::BLOCK_SIZE * 23 / 2 - Common::BLOCK_SIZE;
    Y_COORDINATE = 10;
    player = new Player(X_COORDINATE, Y_COORDINATE);;
}

Tetris_Practice::~Tetris_Practice() {
    delete player; player = nullptr;
}

STATUS_CODE Tetris_Practice::start() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

restartPractice:
    player->start();

    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    // Fade in: change menu
    {
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();
        
        scene->drawChangeMenu(window, true);
    }
    
    // Countdown:
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

    player->setTimer();

    while (not player->isGameOver()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);
                
                goto quitPractice;
            }
            else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
                player->pauseTimer();

                STATUS_CODE escapeOption = scene->drawPause(window);

                switch (escapeOption) {
                    case STATUS_CODE::RESUME:
                        player->unPauseTimer();
                        
                        break;
                    case STATUS_CODE::RESTART:
                        screenStatus = STATUS_CODE::RESTART;
                        
                        goto quitPractice;
                    case STATUS_CODE::MENU:
                        screenStatus = STATUS_CODE::MENU;
                        
                        goto quitPractice;
                    case STATUS_CODE::QUIT:
                        screenStatus = STATUS_CODE::QUIT;
                        
                        goto quitPractice;
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
        window->display();
    
        if (player->isGameOver()) {  // Game over
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            window->display();

            screenStatus = scene->drawGameOver(window);
        }
    }

quitPractice:
    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartPractice;
    }

    return screenStatus;
}
