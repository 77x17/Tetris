#include "TetrisTwoPlayer.hpp"

#include <thread>

#include "Common.hpp"
#include "PlayerWithNetwork.hpp"
#include "Competitor.hpp"
#include "Scene.hpp"
#include "Menu.hpp"

TetrisTowPlayer::TetrisTowPlayer(sf::RenderWindow* win, Scene* s, bool isHost):TetrisBaseMode(win, s), isHost(isHost) {
    player = nullptr; competitor = nullptr;
}

TetrisTowPlayer::~TetrisTowPlayer() {
    delete player; player = nullptr; 
    delete competitor; competitor = nullptr;
}


void TetrisTowPlayer::makeConnection() {
    int PLAYER_X_COORDINATE = window->getSize().x / 4 - BLOCK_SIZE * 23 / 2;
    int PLAYER_Y_COORDINATE = 10;
    int COMPETITOR_X_COORDINATE = 3 * window->getSize().x / 4 - BLOCK_SIZE * 23 / 2;
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
        // competitor = new Competitor(COMPETITOR_X_COORDINATE, COMPETITOR_Y_COORDINATE, "10.0.142.133", 55001);
        // player = new PlayerWithNetwork(PLAYER_X_COORDINATE, PLAYER_Y_COORDINATE, "10.0.142.133", 55000);
    }
    isFinish.store(true);
}

STATUS_CODE TetrisTowPlayer::start() {
    PlayerWithNetwork* player = nullptr;
    Competitor* competitor    = nullptr;

    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    {
        isFinish.store(false);
        std::thread connectThread(&TetrisTowPlayer::makeConnection, this, isHost, 
                                        std::ref(competitor), std::ref(player));
    
        screenStatus = scene->waitingForConnection(window, isFinish);
        // if (connectStatus == -1) { // Error - exit
        //     delete player;
        //     delete competitor;
            
        //     return STATUS_CODE::BACK;
        // }
        connectThread.join();
    }

    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    loadPlayground(backgroundTexture, backgroundSprite);

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
                screenStatus = scene->waitingForConnection(window, isFinish);
                // if (connectStatus == -1) { // Error - exit
                //     delete player;
                //     delete competitor; 
                //     return STATUS_CODE::MENU;
                // }
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

    return STATUS_CODE::MENU;
}