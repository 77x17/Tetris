#include "Tetris_Multiplayer.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <thread>

#include "Common.hpp"
#include "Player_Multiplayer.hpp"
#include "Competitor.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include <iostream>
Tetris_Multiplayer::Tetris_Multiplayer(sf::RenderWindow* win, Scene* s, bool isHost):Tetris(win, s), isHost(isHost) {
    player = nullptr; competitor = nullptr;
}

Tetris_Multiplayer::~Tetris_Multiplayer() {
    delete player; player = nullptr; 
    delete competitor; competitor = nullptr;
}


STATUS_CODE Tetris_Multiplayer::makeConnection() {
    std::atomic<bool> isFinish(false);
    std::thread connectThread([this](std::atomic<bool> &isFinish) {
        int PLAYER_X_COORDINATE = window->getSize().x / 4 - Common::BLOCK_SIZE * 23 / 2;
        int PLAYER_Y_COORDINATE = 10;
        int COMPETITOR_X_COORDINATE = 3 * window->getSize().x / 4 - Common::BLOCK_SIZE * 23 / 2;
        int COMPETITOR_Y_COORDINATE = 10;
        
        if (isHost) {
            sf::TcpListener listener;
            listener.listen(55001);
            std::random_device rd;
            int seed = rd();
            player = new Player_Multiplayer(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, listener, seed);
            listener.listen(55000);
            competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, listener, seed);
        }
        else {
            competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, "127.0.0.1", 55001);
            player = new Player_Multiplayer(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, "127.0.0.1", 55000);
            // competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, "10.29.160.118", 55001);
            // player = new Player_Multiplayer(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, "10.29.160.118", 55000);
        }
        isFinish.store(true);
    }, ref(isFinish));
    STATUS_CODE screenStatus = scene->waitingForConnection(window, isFinish);
    connectThread.join();
    return screenStatus;
}


STATUS_CODE Tetris_Multiplayer::restartGame() {
    std::atomic<bool> isFinish(false);
    std::thread RestartGame([this](std::atomic<bool> &isFinish) {
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
    }, ref(isFinish));
    STATUS_CODE screenStatus = scene->waitingForConnection(window, isFinish);
    RestartGame.join();
    return screenStatus;
}

STATUS_CODE Tetris_Multiplayer::start() {
    STATUS_CODE screenStatus = makeConnection();
    if (screenStatus != STATUS_CODE::RESUME)
        return screenStatus;

    player->setTimer();
    // competitor->setTimer();

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
            STATUS_CODE option = scene->drawGameOver(window);
            player->waitingComfirm();

            if (option == STATUS_CODE::RESTART) {
                screenStatus = restartGame();
                if (screenStatus != STATUS_CODE::RESUME)
                    return screenStatus;

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