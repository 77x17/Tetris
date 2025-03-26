#include "MovementController.hpp"

#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"
#include "Monitor.hpp"

#include "CurrentBlockController.hpp"

#include <iostream>

constexpr float DROP_TIME           = 0.5f;
constexpr float COLLISION_DROP_TIME = 2.5f;

constexpr float DELAY_MOVING_TIME   = 175.0f;
constexpr float MOVING_TIME         = 30.0f;
          float movingTime          = DELAY_MOVING_TIME;

MovementController::MovementController(Monitor* moni, CurrentBlockController* cur): collision(false), moveLeftSignal(false), 
                                                                                    moveRightSignal(false), moveDownSignal(false), 
                                                                                    monitor(moni), curBlock(cur) {}

MovementController::~MovementController() {}

void MovementController::resetComponent() {
    collision = false; moveLeftSignal = false; moveLeftSignal = false;
    moveRightSignal = false; moveDownSignal = false;
}

void MovementController::handleLeft() {
    if (curBlock->moveLeft() and curBlock->collisionBottom() and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handleRight() {
    if (curBlock->moveRight() and curBlock->collisionBottom() and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handleDown() {
    if (curBlock->moveDown() and not curBlock->collisionBottom()) {
        clock.restart();
    }
}

void MovementController::handleUp() {
    if (curBlock->rotateLeft() and curBlock->collisionBottom() and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handlePut() {
    int nLines = curBlock->putIntoMap();
    
    monitor->addLineToInfor(nLines, curBlock->getCurrentBlock());

    collision = false;
    curBlock->setter((monitor->getNext())->updateNext());
    monitor->unlockHold();
    if (curBlock->gameOver())
        monitor->setGameOver();
}

void MovementController::handleHardDrop() {
    curBlock->hardDrop(); 

    handlePut();

    clock.restart();
}

void MovementController::handleHold() {
    if (monitor->canHold()) {
        monitor->exchangeCurrentBlock(curBlock->getCurrentBlock());
        // monitor->exchangeCurrentBlock(curBlock->getCurrentBlock());
        // if (curBlock->isEmpty()) {
        //     curBlock->setter((monitor->getNext())->updateNext());
        // }
        // curBlock->resetPosition();

        clock.restart();
    }
}

void MovementController::processEvents(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left and moveLeftSignal == false) {
            moveLeftSignal  = true;
            moveRightSignal = false;

            handleLeft();
            
            movingTime = DELAY_MOVING_TIME;

            movingClock.restart();
        }
        else if (event.key.code == sf::Keyboard::Right and moveRightSignal == false) {
            moveLeftSignal  = false;
            moveRightSignal = true;

            handleRight();

            movingTime = DELAY_MOVING_TIME;

            movingClock.restart();
        }
        else if (event.key.code == sf::Keyboard::Down and moveDownSignal == false) {
            moveDownSignal = true;

            handleDown();

            movingClock.restart();
        } 
        else if (event.key.code == sf::Keyboard::Up or event.key.code == sf::Keyboard::X) {
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
            moveLeftSignal = false;
            
            movingTime = DELAY_MOVING_TIME;
        }
        else if (event.key.code == sf::Keyboard::Right) {
            moveRightSignal = false;
            
            movingTime = DELAY_MOVING_TIME;
        } 
        else if (event.key.code == sf::Keyboard::Down) {
            moveDownSignal = false;
        }
    }
}

void MovementController::autoDown() {
    if (movingClock.getElapsedTime().asMilliseconds() >= movingTime) {
        if (moveLeftSignal) {
            handleLeft();
            movingTime = MOVING_TIME;
        }
        else if (moveRightSignal) {
            handleRight();
            movingTime = MOVING_TIME;
        }
        
        if (moveDownSignal) {
            handleDown();
            movingTime = MOVING_TIME;
        }
        
        movingClock.restart();
    }

    if (not curBlock->collisionBottom()) {
        collision = false;
    }

    if (clock.getElapsedTime().asSeconds() >= (collision ? COLLISION_DROP_TIME : DROP_TIME)) {
        if (not curBlock->fallDown()) {
            handlePut();
        }
        
        clock.restart();
    }
}