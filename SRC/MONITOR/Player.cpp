#include "Player.hpp"

Player::Player(){}

Player::Player(WINDOW* win):Monitor(win){}

void Player::start() {
    screenPlayer->drawMap(map);
    int cnt = 10;

    while (cnt) {
        if (moveProcessing()) {
            cnt--;
            cur->draw(screenPlayer, '#');
            continue;
        }
    }
}