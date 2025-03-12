#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

constexpr float DROP_TIME = 0.5f; 

Player::Player(sf::RenderWindow *window) : Monitor(window) {}

void Player::start() {
    next->draw(window); 
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
    curBlock->draw(window);

    while (window->isOpen()) {
        processEvents();
        render();
        update();
    }
}

void Player::update() {
    if (clock.getElapsedTime().asSeconds() >= DROP_TIME) {
        if (not curBlock->moveDown(map)) {
            curBlock->put(map);

            curBlock->setter(next->updateNext());
            curBlock->resetPosition(map);
            
            if (curBlock->gameOver(map)) {
                restart();
            }
            
            hold->unlock();
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
    hold    ->draw(window);
    next    ->draw(window);
    map     ->draw(window);
     
    window->display();
}