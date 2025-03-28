#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"
#include "Monitor.hpp"

#include "MonitorForOnePlayer.hpp"
#include "CurrentBlockController.hpp"
#include "MovementController.hpp"

#include <iostream>

Player::Player(int x, int y): volume(50.0f), X_COORDINATE(x), Y_COORDINATE(y){
    soundManager = new SoundManager();
    soundManager->loadSound("scroll", "ASSETS/sfx/scroll.mp3");
}

void Player::initialize() {
    monitor = new MonitorForOnePlayer(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlockController(dynamic_cast<MonitorForOnePlayer*>(monitor)->getMap());
    movementController = new MovementController(monitor, curBlock);
}

Player::~Player() {
    delete monitor; monitor = nullptr;
    delete curBlock; curBlock = nullptr;
    delete soundManager; soundManager = nullptr;
    delete movementController; movementController = nullptr;
}

void Player::resetComponent() {
    movementController->resetComponent();
    curBlock->setter((monitor->getNext())->updateNext());
    monitor->unlockHold();
}

bool Player::isGameOver() {
    return monitor->isGameOver();
}

void Player::start() {
    initialize();
    restart();
}

// void Player::start(int seed) {
//     next->setSeed(seed);
//     resetComponent();
// }

void Player::restart() {
    std::random_device rd;
    monitor->resetMonitor(rd());
    resetComponent();
}

void Player::processEvents(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        movementController->processEvents(event);
    }
    else if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            SoundManager::setVolume(volume);
            soundManager->play("scroll");

            if (event.mouseWheelScroll.delta > 0) {  // Scroll up
                volume += 5.0f;
            } else {  // Scroll down
                volume -= 5.0f;
            }

            // Clamp volume between 0 and 100
            if (volume > 100.0f) volume = 100.0f;
            if (volume < 0.0f) volume = 0.0f;
        }
    }
}

void Player::autoDown() {
    movementController->autoDown();
}

void Player::draw(sf::RenderWindow* window) {
    monitor->draw(window, curBlock->getCurrentBlock());
    // (monitor->getInfor())->drawAudio(window, SoundManager::getVolume());
}