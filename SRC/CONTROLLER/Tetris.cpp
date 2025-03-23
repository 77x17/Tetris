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
                screenStatus = startGameOnePlayer();
                break;
            case STATUS_CODE::VERSUSBOT:
                screenStatus = startGameVersusBot();
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

restartGameOnePlayer:
    int X_COORDINATE = 50 + WINDOW_WIDTH / 4 - BLOCK_SIZE;
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

        scene->drawCountdown(window, 
            (GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE / 2 - WIDTH_BORDER),
            (GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE / 2 - WIDTH_BORDER),
            -1,
            -1
        );
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

    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartGameOnePlayer;
    }

    return screenStatus;
}

STATUS_CODE Tetris::startGameVersusBot() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite, backgroundMusic);
    backgroundMusic.play();

restartGameVersusBot:
    int PLAYER_X_COORDINATE = 50;
    int PLAYER_Y_COORDINATE = 10;
    int BOT_X_COORDINATE = 50 + WINDOW_WIDTH / 2 - 25;
    int BOT_Y_COORDINATE = 10;
    Player *player = new Player(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE);
    Bot *bot = new Bot(BOT_X_COORDINATE, BOT_Y_COORDINATE);
    
    std::random_device rd;
    int seed = rd();
    player->start(seed);
    bot   ->start(seed);

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
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);

                goto quitGameVersusBot;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    STATUS_CODE escapeOption = scene->drawPause(window);

                    switch (escapeOption) {
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

        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        bot   ->draw(window);
        window->display();

        player->autoDown();
        bot   ->autoDown();

        if (player->isGameOver()) {
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            bot   ->draw(window);
            window->display();

            screenStatus = scene->drawGameOver(window);
        }

        backgroundMusic.setVolume(SoundManager::getVolume() - 20);
    }

quitGameVersusBot:
    delete player;
    delete bot;

    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartGameVersusBot;
    }

    return screenStatus;
}

void Tetris::makeConnection(bool isHost, Competitor* &competitor,PlayerWithNetwork* &player) {
    int PLAYER_X_COORDINATE = 50;
    int PLAYER_Y_COORDINATE = 10;
    int COMPETITOR_X_COORDINATE = 50 + WINDOW_WIDTH / 2 - 25;
    int COMPETITOR_Y_COORDINATE = 10;
    
    if (isHost) {
        sf::TcpListener listener;
        listener.listen(55001);
        std::random_device rd;
        int seed = rd();
        
        player = new PlayerWithNetwork(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, listener, seed);
        listener.listen(55000);
        competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, listener, seed);
    }
    else {
        competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, "127.0.0.1", 55001);
        player = new PlayerWithNetwork(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, "127.0.0.1", 55000);
        // competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, "10.0.100.230", 55001);
        // player = new PlayerWithNetwork(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, "10.0.100.230", 55000);
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
            player->sendCurBlock();
            player->autoDown();
            
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            competitor->draw(window);
            window->display();
        }
        else {
            backgroundMusic.setVolume(SoundManager::getVolume() - 20);
            STATUS_CODE option = scene->drawGameOver(window);
            player->waitingComfirm();

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
                window->close();
            }
            else if (option == STATUS_CODE::QUIT) {    // Quit
                window->close();
            }
        }
    }

    delete player;
    delete competitor;
}