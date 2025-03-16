#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

Player::Player(int X_COORDINATE, int Y_COORDINATE):Monitor(X_COORDINATE, Y_COORDINATE) {
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

void Player::start() {
    // next->draw(window); 
    // curBlock->draw(window, map);

    // while (window->isOpen()) {
        // processEvents();
        // render();
        // update();
    // }
}
