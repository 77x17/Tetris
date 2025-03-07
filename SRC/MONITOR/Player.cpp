#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

Player::Player(sf::RenderWindow *window) : Monitor(window) {}

void Player::start() {
    hold->unlock();
    next->draw(); 
    curBlock->draw(map);

    while (window->isOpen()) {
        processEvents();
        update();
        render();
    }
}