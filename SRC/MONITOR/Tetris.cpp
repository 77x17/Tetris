#include "Tetris.hpp"

#include "Player.hpp"
#include "Competitor.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <thread>

const int WINDOW_WIDTH  = 1000;
const int WINDOW_HEIGHT = 700;

Tetris::Tetris() {
    player = nullptr;
    competitor = nullptr;
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetr.io");
}

Tetris::~Tetris() {
    delete player;
    delete competitor;
    delete window;
}

void Tetris::startGameOnePlayer() {
    player = new Player(10, 50);
    Player *mirror = new Player(500, 50);
    
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                return;
            player->processEvents(event);
            mirror->processEvents(event);
        }

        player->autoDown();
        mirror->autoDown();

        window->clear();
        player->draw(window);
        mirror->draw(window);   
        window->display();
    }
}

void Tetris::makeConnection(bool isHost) {
    if (isHost) {
        sf::TcpListener listener;
        listener.listen(55001);
        player = new Player(10, 50, listener);
        listener.listen(55000);
        competitor = new Competitor(500, 50, listener);
    }
    else {
        competitor = new Competitor(500, 50, "127.0.0.1", 55001);
        player = new Player(10, 50, "127.0.0.1", 55000);
    }
    isFinish.store(true);
}

#include <iostream>

void Tetris::startGameTwoPlayer(bool isHost) {
    isFinish.store(false);
    std::thread connectThread(&Tetris::makeConnection, this, isHost);
    connectThread.detach();

    while (window->isOpen() && !isFinish) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                return;
        }
    }

    competitor->initPollEvent();

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                return;
            player->processEvents(event);
            player->sendEvent(event);
        }
        
        player->autoDown();
        competitor->start();

        window->clear();
        player->draw(window);
        competitor->draw(window);
        window->display();
    }
}