#include "Tetris.hpp"

#include "TetrisOnePlayer.hpp"
#include "TetrisTwoPlayer.hpp"
#include "SoundManager.hpp"
#include "Menu.hpp"
#include "Scene.hpp"

#include <SFML/Graphics.hpp>
#include <windows.h>

const int WINDOW_WIDTH  = 1100;
const int WINDOW_HEIGHT = 600;

float SoundManager::volume      = 50.0f;
float SoundManager::musicVolume =  0.0f;
std::unordered_map<std::string, sf::SoundBuffer> SoundManager::musicBuffers = std::unordered_map<std::string, sf::SoundBuffer>();
std::unordered_map<std::string, sf::Sound>       SoundManager::musicSounds  = std::unordered_map<std::string, sf::Sound>();

Tetris::Tetris() {
    // sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // window = new sf::RenderWindow(desktop, "Tetris", sf::Style::None); // Không viền

    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");
    scene  = new Scene(window);
}

Tetris::~Tetris() {
    delete scene; scene = nullptr;
    delete window; window = nullptr;
}

void Tetris::start() {
    while (window->isOpen()) {       
        STATUS_CODE gameType = scene->drawMenu(window);
        
        STATUS_CODE screenStatus = STATUS_CODE::QUIT;
        switch (gameType) {
            case STATUS_CODE::PRACTICE:
                screenStatus = TetrisOnePlayer(window, scene).start();
                break;
            case STATUS_CODE::VERSUSBOT:
                // screenStatus = startGameVersusBot();
                break;
            case STATUS_CODE::MULTIPLAYER_SERVER:
                screenStatus = TetrisTwoPlayer(window, scene, true).start();
                break;
            case STATUS_CODE::MULTIPLAYER_CLIENT:
                screenStatus = TetrisTwoPlayer(window, scene, false).start();
                break;
            case STATUS_CODE::QUIT:
                window->close();
                break;
            default:
                throw std::runtime_error("[Tetris.cpp] - start(): STATUS_CODE error");
        }
        
        switch (screenStatus) {
            case STATUS_CODE::MENU:
                break;
            case STATUS_CODE::QUIT:
                window->close();
                break;
            default:
                throw std::runtime_error("[Tetris.cpp] - start(): STATUS_CODE error");
        }
    }
}

/* Tetris for bot;
STATUS_CODE Tetris::startGameVersusBot() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

restartGameVersusBot:
    int PLAYER_X_COORDINATE = window->getSize().x / 4 - BLOCK_SIZE * 23 / 2;
    int PLAYER_Y_COORDINATE = 10;
    int BOT_X_COORDINATE = 3 * window->getSize().x / 4 - BLOCK_SIZE * 23 / 2;
    int BOT_Y_COORDINATE = 10;
    Player *player = new Player(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE);
    Bot *bot = new Bot(BOT_X_COORDINATE, BOT_Y_COORDINATE);
    
    player->start();
    bot   ->start();

    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    // Fade in: change menu
    {
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        bot   ->draw(window);
        window->display();

        scene->drawChangeMenu(window, true);
    }

    {
        int HOLD_WIDTH         = 5;

        int GRID_WIDTH         = 10;
        int GRID_HEIGHT        = 24;
        int P_GRID_POSITION_X    = PLAYER_X_COORDINATE + HOLD_WIDTH * BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE;
        int P_GRID_POSITION_Y    = PLAYER_Y_COORDINATE;
        int B_GRID_POSITION_X    = BOT_X_COORDINATE + HOLD_WIDTH * BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE;
        int B_GRID_POSITION_Y    = BOT_Y_COORDINATE;

        scene->drawCountdown(window, 
            (P_GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE / 2 - WIDTH_BORDER),
            (P_GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE / 2 - WIDTH_BORDER),
            (B_GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE / 2 - WIDTH_BORDER),
            (B_GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE / 2 - WIDTH_BORDER)
        );
    }

    while (not player->isGameOver()) {
        if (notFocus(window)) { continue; }

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);

                goto quitGameVersusBot;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    STATUS_CODE escapeOption = scene->drawPause(window);

                    switch (escapeOption)    {
                        case STATUS_CODE::RESUME:
                            break;
                        case STATUS_CODE::RESTART:
                            screenStatus = STATUS_CODE::RESTART;
                            
                            goto quitGameVersusBot;
                        case STATUS_CODE::MENU:
                            screenStatus = STATUS_CODE::MENU;
                            
                            goto quitGameVersusBot;
                        case STATUS_CODE::QUIT:
                            screenStatus = STATUS_CODE::QUIT;
                            
                            goto quitGameVersusBot;
                        default:
                            throw std::runtime_error("[escapeOption] cannot find STATUS_CODE");
                    }
                }
            }

            player->processEvents(event);
            bot   ->processEvents(event);
        }

        player->autoDown();
        bot   ->autoDown();

        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        bot   ->draw(window);
        window->display();

        if (player->isGameOver()) {
            // Don't ya need it ?
            // window->clear();
            // window->draw(backgroundSprite); // Draw background
            // player->draw(window);
            // bot   ->draw(window);
            // window->display();

            screenStatus = scene->drawGameOver(window);
        }
    }

quitGameVersusBot:
    delete player;
    delete bot;

    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartGameVersusBot;
    }

    return screenStatus;
}
*/