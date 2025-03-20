#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"

#include <iostream>

constexpr float DROP_TIME           = 0.5f;
constexpr float COLLISION_DROP_TIME = 2.5f;

constexpr float DELAY_MOVING_TIME   = 175.0f;
constexpr float MOVING_TIME         = 30.0f;
          float movingTime          = DELAY_MOVING_TIME;

Player::Player(int X_COORDINATE, int Y_COORDINATE) : Monitor(X_COORDINATE, Y_COORDINATE), 
        volume(50.0f), moveLeftSignal(false), moveRightSignal(false), moveDownSignal(false), 
        collision(false) {
    curBlock = new CurrentBlock();
    
    soundManager = new SoundManager();
    soundManager->loadSound("scroll", "ASSETS/sfx/scroll.mp3");
}

Player::~Player() {
    delete curBlock; curBlock = nullptr;
    
    delete soundManager;
}

void Player::resetComponent() {
    collision = false;
    curBlock->freeAndSetter(next->updateNext());
    curBlock->resetPosition(map);
    hold->unlock();
}

void Player::start() {
    std::random_device rd; 
    next->setSeed(rd());
    resetComponent();
}

void Player::restart() {
    std::random_device rd;
    resetMonitor(rd());
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
    int nLines = curBlock->put(map);
    // playSoundRemoveLine -> playSound and update
    infor->playSoundRemoveLine(nLines, curBlock->spin, curBlock->getTypeBlock());
    infor->removeLine(nLines);

    resetComponent();
    if (curBlock->gameOver(map))
        setGameOver();
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
    if (clock.getElapsedTime().asSeconds() >= (collision ? COLLISION_DROP_TIME : DROP_TIME)) {
        if (not curBlock->fallDown(map)) {
            handlePut();
        }
        
        clock.restart();
    }
}

void Player::draw(sf::RenderWindow* window) {
    Monitor::draw(window);
    
    curBlock->draw(window, map);

    infor->drawAudio(window, SoundManager::getVolume());
}