#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"
#include "Infor.hpp"
#include "SoundManager.hpp"
#include "Monitor.hpp"

#include "CurrentBlockController.hpp"
#include "MovementController.hpp"


// constexpr float DROP_TIME           = 0.5f;
// constexpr float COLLISION_DROP_TIME = 2.5f;

// constexpr float DELAY_MOVING_TIME   = 175.0f;
// constexpr float MOVING_TIME         = 30.0f;
//           float movingTime          = DELAY_MOVING_TIME;

Player::Player(int X_COORDINATE, int Y_COORDINATE): Monitor(X_COORDINATE, Y_COORDINATE), volume(50.0f), collision(false){
    // monitor = new Monitor(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlockController();
    soundManager = new SoundManager();
    movementController = new MovementController();
    soundManager->loadSound("scroll", "ASSETS/sfx/scroll.mp3");
}

Player::~Player() {
    delete curBlock; curBlock = nullptr;
    delete soundManager; soundManager = nullptr;
    delete movementController; movementController = nullptr;
}

void Player::resetComponent() {
    collision = false;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
    hold->unlock();
}

void Player::start() {
    std::random_device rd; 
    next->setSeed(rd());
    resetComponent();
}

void Player::start(int seed) {
    next->setSeed(seed);
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
    int nLines = curBlock->putIntoMap(map);
    
    infor->update(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    infor->playSound(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
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
            curBlock->setter(next->updateNext());
        }
        curBlock->resetPosition(map);

        clock.restart();
    }
}

void Player::processEvents(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        movementController->processEvents(event, curBlock, map, infor, hold, next);
        if(curBlock->gameOver(map))
            setGameOver();
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
    movementController->autoDown(curBlock, map, infor, hold, next);
    // if (movingClock.getElapsedTime().asMilliseconds() >= movingTime) {
    //     if (moveLeftSignal) {
    //         handleLeft();
    //         movingTime = MOVING_TIME;
    //     }
    //     else if (moveRightSignal) {
    //         handleRight();
    //         movingTime = MOVING_TIME;
    //     }
        
    //     if (moveDownSignal) {
    //         handleDown();
    //         movingTime = MOVING_TIME;
    //     }
        
    //     movingClock.restart();
    // }

    // if (not curBlock->collisionBottom(map)) {
    //     collision = false;
    // }

    // if (clock.getElapsedTime().asSeconds() >= (collision ? COLLISION_DROP_TIME : DROP_TIME)) {
    //     if (not curBlock->fallDown(map)) {
    //         handlePut();
    //     }
        
    //     clock.restart();
    // }
}

void Player::draw(sf::RenderWindow* window) {
    Monitor::draw(window);
    
    curBlock->draw(window, map);

    infor->drawAudio(window, SoundManager::getVolume());
}