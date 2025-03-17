#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"
#include "Infor.hpp"

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

constexpr float DROP_TIME           = 0.5f;
constexpr float COLLISION_DROP_TIME = 2.5f;

constexpr float DELAY_MOVING_TIME   = 200.0f;
constexpr float MOVING_TIME         = 30.0f;
          float movingTime          = DELAY_MOVING_TIME;

Player::Player(int X_COORDINATE, int Y_COORDINATE):Monitor(X_COORDINATE, Y_COORDINATE) {
    moveLeft = moveDown = moveRight = false;
    collision = false;
}

Player::~Player() {
}

void Player::resetComponent() {
    collision = false; gameOver = false;
    curBlock->freeAndSetter(next->updateNext());
    curBlock->resetPosition(map);
}

void Player::start() {
    resetComponent();
}

void Player::restart() {
    std::random_device rd;
    clearScreen(rd());
    resetComponent();
}

void Player::handleLeft() {
    if (curBlock->moveLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void Player::handleRight() {
    if (curBlock->moveRight(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void Player::handleDown() {
    if (curBlock->moveDown(map) and not curBlock->collisionBottom(map)) {
        clock.restart();
    }
}

void Player::handleUp() {
    if (curBlock->rotateLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void Player::handlePut() {
    infor->addLine(curBlock->put(map), curBlock->spin);

    curBlock->freeAndSetter(next->updateNext());
    curBlock->resetPosition(map); 
    collision = false;
    gameOver = curBlock->gameOver(map);
    hold->unlock();
}

void Player::handleHardDrop() {
    curBlock->hardDrop(map); 

    handlePut();

    clock.restart();
}

void Player::handleHold() {
    if (hold->canHold()) {
        hold->lock();
        curBlock->swap(hold);
        if (curBlock->isEmpty()) {
            curBlock->freeAndSetter(next->updateNext());
        }
        curBlock->resetPosition(map);

        clock.restart();
    }
}

void Player::processEvents(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left and curBlock->moveLeftSignal == false) {
            curBlock->moveLeftSignal  = true;
            curBlock->moveRightSignal = false;

            handleLeft();
            
            movingTime = DELAY_MOVING_TIME;

        movingClock.restart();
    }
    else if (event.key.code == sf::Keyboard::Right and moveRight == false) {
        moveLeft  = false;
        moveRight = true;
            movingClock.restart();
        }
        else if (event.key.code == sf::Keyboard::Right and curBlock->moveRightSignal == false) {
            curBlock->moveLeftSignal  = false;
            curBlock->moveRightSignal = true;

            handleRight();

            movingTime = DELAY_MOVING_TIME;

        movingClock.restart();
    } 
    else if (event.key.code == sf::Keyboard::Down and moveDown == false) {
        moveDown = true;
            movingClock.restart();
        } 
        else if (event.key.code == sf::Keyboard::Down and curBlock->moveDownSignal == false) {
            curBlock->moveDownSignal = true;

            handleDown();

            movingClock.restart();
        } 
        else if (event.key.code == sf::Keyboard::Up) {
            handleUp();
        } 
        else if (event.key.code == sf::Keyboard::Space) {
            handleHardDrop();
        } 
        else if (event.key.code == sf::Keyboard::C) {
            handleHold();
        }
    } 
    else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Left) {
            curBlock->moveLeftSignal = false;
            
            movingTime = DELAY_MOVING_TIME;
        }
        else if (event.key.code == sf::Keyboard::Right) {
            curBlock->moveRightSignal = false;
            
            movingTime = DELAY_MOVING_TIME;
        } 
        else if (event.key.code == sf::Keyboard::Down) {
            curBlock->moveDownSignal = false;
        } 
    }
}

void Player::autoDown() {
    if (movingClock.getElapsedTime().asMilliseconds() >= movingTime) {
        if (curBlock->moveLeftSignal) {
            handleLeft();
            movingTime = MOVING_TIME;
        }
        else if (curBlock->moveRightSignal) {
            handleRight();
            movingTime = MOVING_TIME;
        }
        
        if (curBlock->moveDownSignal) {
            handleDown();
            movingTime = MOVING_TIME;
        }
        
        movingClock.restart();
    }
    if (clock.getElapsedTime().asSeconds() >= (collision ? COLLISION_DROP_TIME : DROP_TIME)) {
        if (not curBlock->moveDown(map)) {
            handlePut();
        }
        
        clock.restart();
    }
}