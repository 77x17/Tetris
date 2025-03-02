#include "Player.hpp"

Player::Player(){}

Player::Player(WINDOW* win):Monitor(win){}

void Player::start() {
    while (true) {
        if (moveProcessing()) {

            continue;
        }
    }
}