#include "Tetris.hpp"

#include "Player.hpp"
#include "PlayerWithNetwork.hpp"
#include "Competitor.hpp"
#include "SoundManager.hpp"
#include "Common.hpp"
#include "Menu.hpp"
#include "Scene.hpp"
#include "Bot.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <windows.h>
#include <iostream>

const int WINDOW_WIDTH  = 1100;
const int WINDOW_HEIGHT = 600;

float SoundManager::volume      = 0.0f;
float SoundManager::musicVolume =  0.0f;
std::unordered_map<std::string, sf::SoundBuffer> SoundManager::musicBuffers = std::unordered_map<std::string, sf::SoundBuffer>();
std::unordered_map<std::string, sf::Sound>       SoundManager::musicSounds  = std::unordered_map<std::string, sf::Sound>();

Tetris::Tetris() {
    // sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // window = new sf::RenderWindow(desktop, "Tetris", sf::Style::None); // Không viền
    // window = new sf::RenderWindow(desktop, "Tetris"); // Không viền
    // window->setPosition(sf::Vector2i(0, 0)); // Đặt vị trí góc trên cùng bên trái

    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");
    scene  = new Scene(window);
}

Tetris::~Tetris() {
    delete scene; scene = nullptr;
    delete window; window = nullptr;
}

bool Tetris::notFocus(sf::RenderWindow *window) {
    if (not window->hasFocus()) {
        // ShowWindow(window->getSystemHandle(), SW_MINIMIZE);

        // sf::Event event;
        // while (window->pollEvent(event)) {
        //     // nothing
        // }

        // sf::sleep(sf::milliseconds(100));

        return true;
    }

    return false;
}

void Tetris::start() {
    while (window->isOpen()) {       
        STATUS_CODE gameType = scene->drawMenu(window);
        
        STATUS_CODE screenStatus = STATUS_CODE::QUIT;
        switch (gameType) {
            case STATUS_CODE::PRACTICE: 
                screenStatus = startGameOnePlayer();
                break;
            case STATUS_CODE::VERSUSBOT:
                screenStatus = startGameVersusBot();
                break;
            case STATUS_CODE::MULTIPLAYER_SERVER:
                // screenStatus = startGameTwoPlayer(true);   
                break;
            case STATUS_CODE::MULTIPLAYER_CLIENT:
                // screenStatus = startGameTwoPlayer(false);
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

void Tetris::loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite) {
    backgroundTexture.loadFromFile("ASSETS/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 50));

    // Get window size & texture size
    sf::Vector2u windowSize  = window->getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    // Calculate scale factors
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::max(scaleX, scaleY);
    // Apply scale to fit window
    backgroundSprite.setScale(scale, scale);
    // Center the sprite
    float newWidth  = textureSize.x * scale;
    float newHeight = textureSize.y * scale;
    float posX = (windowSize.x - newWidth ) / 2;
    float posY = (windowSize.y - newHeight) / 2;
    backgroundSprite.setPosition(posX, posY);
}

STATUS_CODE Tetris::startGameOnePlayer() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite);
    backgroundMusic.play();


restartGameOnePlayer:
    int X_COORDINATE = window->getSize().x / 2 - BLOCK_SIZE * 23 / 2 - BLOCK_SIZE;
    int Y_COORDINATE = 10;
    Player* player = new Player(X_COORDINATE, Y_COORDINATE);
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

    {
        int HOLD_WIDTH         = 5;

        int GRID_WIDTH         = 10;
        int GRID_HEIGHT        = 24;
        int GRID_POSITION_X    = X_COORDINATE + HOLD_WIDTH * BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE;
        int GRID_POSITION_Y    = Y_COORDINATE;

        // scene->drawCountdown(window, 
        //     (GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE / 2 - WIDTH_BORDER),
        //     (GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE / 2 - WIDTH_BORDER),
        //     -1,
        //     -1
        // );
    }

    sf::Event event;
    while (not player->isGameOver()) {
        while (window->pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);

                goto quitStartGameOnePlayer;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    STATUS_CODE escapeOption = scene->drawPause(window);

                    switch (escapeOption) {
                        case STATUS_CODE::RESUME:
                            break;
                        case STATUS_CODE::RESTART:
                            screenStatus = STATUS_CODE::RESTART;
                            
                            goto quitStartGameOnePlayer;
                        case STATUS_CODE::MENU:
                            screenStatus = STATUS_CODE::MENU;
                            
                            goto quitStartGameOnePlayer;
                        case STATUS_CODE::QUIT:
                            screenStatus = STATUS_CODE::QUIT;
                            
                            goto quitStartGameOnePlayer;
                        default:
                            throw std::runtime_error("[escapeOption] cannot find STATUS_CODE");
                    }
                }
            }

            player->processEvents(event);
        }

        player->autoDown();
        
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

        backgroundMusic.setVolume(SoundManager::getVolume() - 20);
    }

quitStartGameOnePlayer:
    delete player;

    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartGameOnePlayer;
    }

    return screenStatus;
}

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
