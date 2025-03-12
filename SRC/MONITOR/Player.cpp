#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

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
