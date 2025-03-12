#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

Player::Player(sf::RenderWindow *window, int X_COORDINATE, int Y_COORDINATE) : Monitor(window, X_COORDINATE, Y_COORDINATE) {}

void Player::start() {
    next->draw(window); 
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
    curBlock->draw(window, map);

    while (window->isOpen()) {
        processEvents();
        render();
        update();
    }
}
