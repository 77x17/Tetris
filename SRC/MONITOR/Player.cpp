#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

constexpr float DROP_TIME = 0.5f; 

Player::Player(sf::RenderWindow *window) : Monitor(window) {}

void Player::start() {
    hold->unlock();
    next->draw(); 
    curBlock->draw(window);

    while (window->isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Player::update() {
    if (clock.getElapsedTime().asSeconds() >= DROP_TIME) {
        if (not curBlock->moveDown(map)) {
            curBlock->put(map);

            // if (tetromino.gameOver()) {
            //     tetromino = Tetromino();
            //     board     = Board();
            // }
            curBlock->setter(next->updateNext());
        }
        
        clock.restart();
    }
}

void Player::render() {
    window->clear();

    map ->drawOutline(window);
    hold->drawOutline(window);
    next->drawOutline(window);
    curBlock->draw(window);
    // hold    ->draw(map);
    // next    ->draw(map);
    // map->draw();
     
    window->display();
}