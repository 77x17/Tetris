#include "Tetris.hpp"

#include "Player.hpp"
#include "PlayerWithNetwork.hpp"
#include "Competitor.hpp"
#include "SoundManager.hpp"
#include "Common.hpp"
#include "Menu.hpp"
#include "Scene.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

#include <iostream>

const int WINDOW_WIDTH  = 1100;
const int WINDOW_HEIGHT = 700;

float SoundManager::volume = 50.0f;

Tetris::Tetris() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris", sf::Style::Close);
    scene  = new Scene(window);
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

Tetris::~Tetris() {
    delete window;
    delete scene;
}

void Tetris::start() {
    while (window->isOpen()) {       
        STATUS_CODE gameType = scene->drawMenu(window);
        
        STATUS_CODE screenStatus = STATUS_CODE::QUIT;
        switch (gameType) {
            case STATUS_CODE::SINGLEPLAYER: 
                startGameOnePlayer:
                screenStatus = startGameOnePlayer();
                break;
            case STATUS_CODE::VERSUSBOT:
                startGameVersusBot();
                break;
            case STATUS_CODE::MULTIPLAYER_SERVER:
                startGameTwoPlayer(true);   
                break;
            case STATUS_CODE::MULTIPLAYER_CLIENT:
                startGameTwoPlayer(false);
                break;
            case STATUS_CODE::QUIT:
                window->close();
                break;
            default:
                throw std::runtime_error("[Menu.cpp] cannot find STATUS_CODE");
        }
        
        switch (screenStatus) {
            case STATUS_CODE::MENU:
                break;
            case STATUS_CODE::SINGLEPLAYER:
                goto startGameOnePlayer;
            case STATUS_CODE::QUIT:
                window->close();
                break;
            default:
                throw std::runtime_error("[Menu.cpp] cannot find STATUS_CODE");
        }
    }
}

void Tetris::loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite, sf::Music &backgroundMusic) {
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

    backgroundMusic.openFromFile("ASSETS/sfx/_tetristheme.mp3");
    backgroundMusic.setLoop(true);
}

STATUS_CODE Tetris::startGameOnePlayer() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite, backgroundMusic);
    backgroundMusic.play();

    Player* player = new Player(50 + WINDOW_WIDTH / 4 - BLOCK_SIZE, 10);
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

    while (not player->isGameOver()) {
        sf::Event event;
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
                        case STATUS_CODE::MENU:
                            screenStatus = STATUS_CODE::MENU;
                            
                            goto quitStartGameOnePlayer;
                        case STATUS_CODE::QUIT:
                            goto quitStartGameOnePlayer;
                        default:
                            throw std::runtime_error("[escapeOption] cannot find STATUS_CODE");
                    }
                }
            }

            player->processEvents(event);
        }

        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();

        player->autoDown();

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

    return screenStatus;
}

STATUS_CODE Tetris::startGameVersusBot() {
    return STATUS_CODE::MENU;
}

void Tetris::makeConnection(bool isHost, Competitor* &competitor,PlayerWithNetwork* &player) {
    if (isHost) {
        sf::TcpListener listener;
        listener.listen(55001);
        std::random_device rd;
        int seed = rd();
        player = new PlayerWithNetwork(50, 10, listener, seed);
        listener.listen(55000);
        competitor = new Competitor(50 + WINDOW_WIDTH / 2 - 25, 10, listener, seed);
    }
    else {
        competitor = new Competitor(50 + WINDOW_WIDTH / 2 - 25, 10, "127.0.0.1", 55001);
        player = new PlayerWithNetwork(50, 10, "127.0.0.1", 55000);
        // competitor = new Competitor(50 + WINDOW_WIDTH / 2 - 25, 10, "186.186.0.55", 55001);
        // player = new PlayerWithNetwork(50, 10, "186.186.0.55", 55000);
    }
    isFinish.store(true);
}

void Tetris::startGameTwoPlayer(bool isHost) {
    PlayerWithNetwork* player = nullptr;
    Competitor* competitor    = nullptr;
    {
        isFinish.store(false);
        std::thread connectThread(&Tetris::makeConnection, this, isHost, 
                                        std::ref(competitor), std::ref(player));
    
        int connectStatus = scene->waitingForConnection(window, isFinish);
        if (connectStatus == -1) { // Error - exit
            delete player;
            delete competitor;
            
            return;
        }
        connectThread.join();
    }

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite, backgroundMusic);
    backgroundMusic.play();

    competitor->start(player);

    int screenStatus = -1;
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
            player->waitingComfirm();
        }
        if (!player->isGameOver()) {
            player->autoDown();
            player->sendCurBlock();
            
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            competitor->draw(window);
            window->display();
        }
        else {
            // backgroundMusic.setVolume(SoundManager::getVolume() - 20);
            STATUS_CODE option = scene->drawGameOver(window);

            if (option == STATUS_CODE::RESTART) {
                isFinish.store(false);
                std::thread RestartGame([this](PlayerWithNetwork* player, Competitor* competitor, bool isHost) {
                    int seed = 0;
                    if (isHost) {
                        std::random_device rd; seed = rd();
                        player->ready(seed);
                        competitor->ready(seed);
                    }
                    else {
                        competitor->ready(seed);
                        player->ready(seed);
                    }
                    isFinish.store(true);
                }, player, competitor, isHost);
                int connectStatus = scene->waitingForConnection(window, isFinish);
                if (connectStatus == -1) { // Error - exit
                    delete player;
                    delete competitor; 
                    return;
                }
                RestartGame.join();
                competitor->start(player);
            }
            else if (option == STATUS_CODE::MENU) {     // Menu
                screenStatus = 0;
                window->close();
            }
            else if (option == STATUS_CODE::QUIT) {    // Quit
                screenStatus = -1;
                window->close();
            }
        }
    }

    delete player;
    delete competitor;
}