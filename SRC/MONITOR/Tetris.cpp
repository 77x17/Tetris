#include "Tetris.hpp"

#include "Player.hpp"

#include <thread>

const int WINDOW_WIDTH  = 800;
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
    player = new Player(window, 100, 50);
    while (window->isOpen()) {
        player->start();
        
    }

}