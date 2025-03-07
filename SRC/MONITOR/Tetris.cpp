#include "Tetris.hpp"

#include "Player.hpp"

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 700;

Tetris::Tetris() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetr.io");
    player = new Player(window);
}

Tetris::~Tetris() {
    delete player;
    delete window;
}

void Tetris::startGameOnePlayer() {
    player->start();
}