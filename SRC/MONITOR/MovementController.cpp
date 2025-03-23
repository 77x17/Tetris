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

MovementController::MovementController(): collision(false), moveLeftSignal(false), moveRightSignal(false), moveDownSignal(false) {}

MovementController::~MovementController() {}

void MovementController::resetComponent() {
    collision = false; moveLeftSignal = false; moveLeftSignal = false;
    moveRightSignal = false; moveDownSignal = false;
}

void MovementController::handleLeft(CurrentBlockController* curBlock, Map* map) {
    if (curBlock->moveLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handleRight(CurrentBlockController* curBlock, Map* map) {
    if (curBlock->moveRight(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handleDown(CurrentBlockController* curBlock, Map* map) {
    if (curBlock->moveDown(map) and not curBlock->collisionBottom(map)) {
        clock.restart();
    }
}

void MovementController::handleUp(CurrentBlockController* curBlock, Map* map) {
    if (curBlock->rotateLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void MovementController::handlePut(CurrentBlockController* curBlock, Map* map, Hold* hold, Infor* infor, LinkListBlock* next) {
    int nLines = curBlock->putIntoMap(map);
    
    infor->update(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    infor->playSound(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    infor->removeLine(nLines);

    collision = false;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
    hold->unlock();
}

void MovementController::handleHardDrop(CurrentBlockController* curBlock, Map* map, Hold* hold, Infor* infor, LinkListBlock* next) {
    curBlock->hardDrop(map); 

    handlePut(curBlock, map, hold, infor, next);

    clock.restart();
}

void MovementController::handleHold(CurrentBlockController* curBlock, Map* map, Hold* hold, LinkListBlock* next) {
    if (hold->canHold()) {
        hold->lock();
        curBlock->swap(hold);
        if (curBlock->isEmpty()) {
            curBlock->setter(next->updateNext());
        }
        curBlock->resetPosition(map);

        clock.restart();
    }
}

void MovementController::processEvents(const sf::Event &event, CurrentBlockController* curBlock, Map* map, Infor* infor, Hold* hold, LinkListBlock* next) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left and moveLeftSignal == false) {
            moveLeftSignal  = true;
            moveRightSignal = false;

            handleLeft(curBlock, map);
            
            movingTime = DELAY_MOVING_TIME;

            movingClock.restart();
        }
        else if (event.key.code == sf::Keyboard::Right and moveRightSignal == false) {
            moveLeftSignal  = false;
            moveRightSignal = true;

            handleRight(curBlock, map);

            movingTime = DELAY_MOVING_TIME;

            movingClock.restart();
        }
        else if (event.key.code == sf::Keyboard::Down and moveDownSignal == false) {
            moveDownSignal = true;

            handleDown(curBlock, map);

            movingClock.restart();
        } 
        else if (event.key.code == sf::Keyboard::Up or event.key.code == sf::Keyboard::X) {
            handleUp(curBlock, map);
        } 
        else if (event.key.code == sf::Keyboard::Space) {
            handleHardDrop(curBlock, map, hold, infor, next);
        } 
        else if (event.key.code == sf::Keyboard::C) {
            handleHold(curBlock, map, hold, next);
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

void MovementController::autoDown(CurrentBlockController* curBlock, Map* map, Infor* infor, Hold* hold, LinkListBlock* next) {
    if (movingClock.getElapsedTime().asMilliseconds() >= movingTime) {
        if (moveLeftSignal) {
            handleLeft(curBlock, map);
            movingTime = MOVING_TIME;
        }
        else if (moveRightSignal) {
            handleRight(curBlock, map);
            movingTime = MOVING_TIME;
        }
        
        if (moveDownSignal) {
            handleDown(curBlock, map);
            movingTime = MOVING_TIME;
        }
        
        movingClock.restart();
    }

    if (not curBlock->collisionBottom(map)) {
        collision = false;
    }

    if (clock.getElapsedTime().asSeconds() >= (collision ? COLLISION_DROP_TIME : DROP_TIME)) {
        if (not curBlock->fallDown(map)) {
            handlePut(curBlock, map, hold, infor, next);
        }
        
        clock.restart();
    }
}