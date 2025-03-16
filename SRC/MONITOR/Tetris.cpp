#include "Tetris.hpp"

#include "Player.hpp"

#include <thread>

const int WINDOW_WIDTH  = 1000;
const int WINDOW_HEIGHT = 700;

Tetris::Tetris() {
    player = nullptr;
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetr.io");
}

Tetris::~Tetris() {
    delete player;
    delete window;
}

void Tetris::startGameOnePlayer() {
    player = new Player(10, 50);
    Player *mirror = new Player(500, 50);
    
    while (window->isOpen()) {
        sf::Event event;
        if (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                break;
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