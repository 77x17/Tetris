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

#include "KeyConfiguration.hpp"

#include <iostream>

constexpr float DROP_TIME           = 0.5f;
constexpr float COLLISION_DROP_TIME = 2.5f;

constexpr float DELAY_MOVING_TIME   = 175.0f;
constexpr float MOVING_TIME         = 30.0f;
          float movingTime          = DELAY_MOVING_TIME;

MovementController::MovementController(Monitor* moni, CurrentBlockController* cur): collision(false), moveLeftSignal(false), 
                                                                                    moveRightSignal(false), moveDownSignal(false), 
                                                                                    monitor(moni), curBlock(cur) {
    key = new KeyConfiguration("ASSETS/keyBindings.txt");
}

MovementController::~MovementController() {
    delete key;
}

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

void MovementController::handleRotateLeft() {
    if (curBlock->rotateLeft() and curBlock->collisionBottom() and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handleRotateRight() {
    if (curBlock->rotateRight() and curBlock->collisionBottom() and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handleRotate180() {
    if (curBlock->rotate180() and curBlock->collisionBottom() and collision == false) {
        collision = true;

        clock.restart();
    }
}


void MovementController::handlePut() {
    int nLines = monitor->putIntoMap(curBlock->getCurrentBlock());
    monitor->removeNLines(nLines, curBlock->getCurrentBlock());

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
        clock.restart();
    }
}

void MovementController::processEvents(const sf::Event &eventFromKeyboard) {
    switch (key->getEvent(eventFromKeyboard.key.code)) {
        case MOVE_LEFT: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed) {
                if (moveLeftSignal == false) {
                    moveLeftSignal  = true; moveRightSignal = false;
                    handleLeft();
                    movingTime = DELAY_MOVING_TIME;
                    movingClock.restart();
                }
            }
            else {
                moveLeftSignal = false;
                movingTime = DELAY_MOVING_TIME;
            }
        } break;

        case MOVE_RIGHT: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed) {
                if (moveRightSignal == false) {
                    moveLeftSignal  = false; moveRightSignal = true;
                    handleRight();
                    movingTime = DELAY_MOVING_TIME;
                    movingClock.restart();
                }
            }
            else {
                moveRightSignal = false;
                movingTime = DELAY_MOVING_TIME;
            }
        } break;
        
        case MOVE_DOWN: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed) {
                if (moveDownSignal == false) {
                    moveDownSignal = true;
                    handleDown();
                    movingClock.restart();
                }
            }
            else {
                moveDownSignal = false;
            }
        } break;
        
        case ROTATE_CLOCKWISE: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed)
                handleRotateLeft();
        } break;

        case ROTATE_COUNTERCLOCKWISE: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed)
                handleRotateRight();
        } break;

        case ROTATE_180: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed)
                handleRotate180();
        } break;

        case HARD_DROP: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed)
                handleHardDrop();
        } break;

        case HOLD_PIECE: {
            if (eventFromKeyboard.type == sf::Event::KeyPressed)
                handleHold();
        }break;
        default:
            break;
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