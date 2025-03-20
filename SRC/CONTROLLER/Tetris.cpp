#include "Tetris.hpp"

#include "Player.hpp"
#include "PlayerWithNetwork.hpp"
#include "Competitor.hpp"
#include "SoundManager.hpp"
#include "Menu.hpp"
#include "Common.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

#include <iostream>

const int WINDOW_WIDTH  = 1100;
const int WINDOW_HEIGHT = 700;

float SoundManager::volume = 50.0f;

Tetris::Tetris() {
    window = nullptr;
    menu   = new Menu();
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

Tetris::~Tetris() {
    delete window;
    delete menu;
}

void Tetris::start() {
    while (true) {       
        int gameType = menu->createWindow(window);
        
        int screenStatus = 0;
        if (gameType == 0) {
            screenStatus = startGameOnePlayer();
        }
        else if (gameType == 1) {
            startGameTwoPlayer(true);   // Server
        }
        else if (gameType == 2) {
            startGameTwoPlayer(false);  // Client
        }
        else if (gameType == -1) {      // Exit;
            break;
        }

        if (screenStatus == -1) {
            break;
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

int Tetris::startGameOnePlayer() {
    int screenStatus = -1;

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite, backgroundMusic);
    backgroundMusic.play();

    Player* player = new Player(50 + WINDOW_WIDTH / 4 - BLOCK_SIZE, 10);
    player->start();
    
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                screenStatus = -1;
                window->close();
            }

            if (not player->isGameOver()) {
                player->processEvents(event);
            }
        }

        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();

        if (player->isGameOver()) {
            sf::Texture screenshot;
            screenshot.create(window->getSize().x, window->getSize().y);
            screenshot.update(*window);
            int option = menu->drawGameOver(window, screenshot);

            if (option == 0) {          // Restart
                player->restart();
            }
            else if (option == 1) {     // Menu
                screenStatus = 0;
                window->close();
            }
            else if (option == -1) {    // Quit
                screenStatus = -1;
                window->close();
            }
        }
        else {
            player->autoDown();

            backgroundMusic.setVolume(SoundManager::getVolume() - 20);
        }
    }

    delete player;

    return screenStatus;
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
        // competitor = new Competitor(50 + WINDOW_WIDTH / 2 - 25, 10, "10.0.108.217", 55001);
        // player = new PlayerWithNetwork(50, 10, "10.0.108.217", 55000);
    }
    isFinish.store(true);
}

void Tetris::startGameTwoPlayer(bool isHost) {
    PlayerWithNetwork* player = nullptr;
    Competitor* competitor    = nullptr;

    isFinish.store(false);
    std::thread connectThread(&Tetris::makeConnection, this, isHost, 
                                    std::ref(competitor), std::ref(player));
    connectThread.detach();

    int connectStatus = menu->waitingForConnection(window, isFinish);
    if (connectStatus == -1) { // Error - exit
        delete player;
        delete competitor;
        
        return;
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

        player->autoDown();
        
        window->clear();
        player->sendCurBlock();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        competitor->draw(window);
        window->display();

        backgroundMusic.setVolume(SoundManager::getVolume() - 20);

        if (player->isGameOver() || competitor->isGameOver()) {
            sf::Texture screenshot;
            screenshot.create(window->getSize().x, window->getSize().y);
            screenshot.update(*window);
            int option = menu->drawGameOver(window, screenshot);

            if (option == 0) {          // Restart
                std::random_device rd; int seed = rd();
                player->restart(seed);
                competitor->restart(seed);
            }
            else if (option == 1) {     // Menu
                screenStatus = 0;
                window->close();
            }
            else if (option == -1) {    // Quit
                screenStatus = -1;
                window->close();
            }
        }
    }

    delete player;
    delete competitor;
}