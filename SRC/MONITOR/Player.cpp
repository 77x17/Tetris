#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"

Player::Player(WINDOW* win):Monitor(win){}

void Player::start() {
    int cnt = 20;
    next->draw(); 
    curBlock->draw(map);
    while (cnt) {
        if (moveProcessing()) {
            cnt--;
            continue;
        }
    }
}