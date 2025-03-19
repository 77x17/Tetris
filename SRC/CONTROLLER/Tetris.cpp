#include "Tetris.hpp"

#include "Player.hpp"
#include "PlayerWithNetwork.hpp"
#include "Competitor.hpp"
#include "SoundManager.hpp"
#include "Menu.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Graphics.hpp>
#include <thread>

const int WINDOW_WIDTH  = 1050;
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
        
        if (gameType == 0) {
            startGameOnePlayer();
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

    backgroundMusic.openFromFile("ASSETS/sfx/tetristheme.mp3");
    backgroundMusic.setLoop(true);
}

void Tetris::startGameOnePlayer() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetr.io", sf::Style::Close);

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite, backgroundMusic);
    backgroundMusic.play();

    Player* player = new Player(50 + WINDOW_WIDTH / 4 - 25, 10);
    player->start();
    
    while (window->isOpen()) {
        sf::Event event;

        if (player->isGameOver()) {
            player->restart();
        }

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            player->processEvents(event);
        }

        player->autoDown();

        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();

        backgroundMusic.setVolume(SoundManager::getVolume() - 20);
    }

    delete player;
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
        // competitor = new Competitor(550, 10, "10.0.133.113", 55001);
        // player = new PlayerWithNetwork(50, 10, "10.0.133.113", 55000);
        competitor = new Competitor(50 + WINDOW_WIDTH / 2 - 25, 10, "127.0.0.1", 55001);
        player = new PlayerWithNetwork(50, 10, "127.0.0.1", 55000);
    }
    isFinish.store(true);
}

void Tetris::startGameTwoPlayer(bool isHost) {
    PlayerWithNetwork* player = nullptr;
    Competitor* competitor    = nullptr;

    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetr.io " + (std::string)(isHost ? "Server" : "Client"), sf::Style::Close);

    isFinish.store(false);
    std::thread connectThread(&Tetris::makeConnection, this, isHost, 
                                    std::ref(competitor), std::ref(player));
    connectThread.detach();

    {
        sf::Clock waitingClock;
        uint8_t count = 0;
        std::string waiting = "Waiting for another player"; 
        sf::Text waitingText(waiting, font, 40);
        waitingText.setPosition(WINDOW_WIDTH  / 2 - waitingText.getGlobalBounds().width / 2, 
                                WINDOW_HEIGHT / 2 - waitingText.getGlobalBounds().height / 2);
        while (window->isOpen() && !isFinish) {
            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window->close();
                    
                    delete player;
                    delete competitor;

                    return;
                }
            }

            window->clear(sf::Color(30, 30, 30));
            if (waitingClock.getElapsedTime().asSeconds() >= 0.5f) {
                count++;

                if (count == 4) {
                    count = 0;
                    waiting.erase(waiting.end() - 6, waiting.end());

                    waitingText.setString(waiting);
                } 
                else {
                    waitingText.setString(waiting += " .");
                }

                waitingClock.restart();
            }
            window->draw(waitingText);
            window->display();
        }
    }

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite, backgroundMusic);
    backgroundMusic.play();

    competitor->start();

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
    }

    delete player;
    delete competitor;
}