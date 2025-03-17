#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

constexpr float DROP_TIME           = 0.5f;
constexpr float COLLISION_DROP_TIME = 2.5f;

constexpr float DELAY_MOVING_TIME   = 200.0f;
constexpr float MOVING_TIME         = 30.0f;
          float movingTime          = DELAY_MOVING_TIME;

Monitor::Monitor(int x, int y) : X_COORDINATE(x), Y_COORDINATE(y) {
    
    moveLeft = moveDown = moveRight = false;

    int HOLD_WIDTH       = 5;
    int HOLD_HEIGHT      = 3;
    int HOLD_POSITION_X  = X_COORDINATE;
    int HOLD_POSITION_Y  = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int GRID_WIDTH       = 10;
    int GRID_HEIGHT      = 24;
    int GRID_POSITION_X  = HOLD_POSITION_X + BLOCK_SIZE * (HOLD_WIDTH + 1);
    int GRID_POSITION_Y  = Y_COORDINATE;
    
    int NEXT_WIDTH       = 5;
    int NEXT_HEIGHT      = 15;
    int NEXT_POSITION_X  = GRID_POSITION_X + BLOCK_SIZE * (GRID_WIDTH + 1);
    int NEXT_POSITION_Y  = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int INFOR_POSITION_X = HOLD_POSITION_X;
    int INFOR_POSITION_Y = HOLD_POSITION_Y + (HOLD_HEIGHT + 1) * BLOCK_SIZE;
    int INFOR_WIDTH      = 5;
    
    hold      = new Hold(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map       = new Map(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next      = new LinkListBlock(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    infor     = new Infor(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * BLOCK_SIZE);
    curBlock  = new CurrentBlock();
    collision = false;
}

Monitor::~Monitor() {
    if (curBlock) { delete curBlock; curBlock = nullptr; }
    if (hold)     { delete hold;     hold     = nullptr; }
    if (next)     { delete next;     next     = nullptr; }
    if (map)      { delete map;      map      = nullptr; }
    if (infor)    { delete infor;    infor    = nullptr; }
}

void Monitor::handleLeft() {
    if (curBlock->moveLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void Monitor::handleRight() {
    if (curBlock->moveRight(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void Monitor::handleDown() {
    if (curBlock->moveDown(map) and not curBlock->collisionBottom(map)) {
        clock.restart();
    }
}

void Monitor::handleUp() {
    if (curBlock->rotateLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
    }
}

void Monitor::handlePut() {
    infor->addLine(curBlock->put(map), curBlock->spin);

    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map); 
    collision = false;

    if (curBlock->gameOver(map)) {
        restart();
    }

    hold->unlock();
}

void Monitor::handleHardDrop() {
    curBlock->hardDrop(map); 

    handlePut();

    clock.restart();
}

void Monitor::handleHold() {
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

void Monitor::processEvents(const sf::Event &event) {
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

void Monitor::autoDown() {
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

void Monitor::draw(sf::RenderWindow* window) {
    map ->drawOutline(window);
    hold->drawOutline(window);
    next->drawOutline(window);
    curBlock->draw(window, map);
    hold    ->draw(window);
    next    ->draw(window);
    map     ->draw(window);
    infor   ->draw(window);
}

void Monitor::restart() {
    delete hold;
    delete map;
    delete next;
    delete infor;

    int HOLD_WIDTH       = 5;
    int HOLD_HEIGHT      = 3;
    int HOLD_POSITION_X  = X_COORDINATE;
    int HOLD_POSITION_Y  = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int GRID_WIDTH       = 10;
    int GRID_HEIGHT      = 24;
    int GRID_POSITION_X  = HOLD_POSITION_X + BLOCK_SIZE * (HOLD_WIDTH + 1);
    int GRID_POSITION_Y  = Y_COORDINATE;
    
    int NEXT_WIDTH       = 5;
    int NEXT_HEIGHT      = 15;
    int NEXT_POSITION_X  = GRID_POSITION_X + BLOCK_SIZE * (GRID_WIDTH + 1);
    int NEXT_POSITION_Y  = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int INFOR_POSITION_X = HOLD_POSITION_X;
    int INFOR_POSITION_Y = HOLD_POSITION_Y + (HOLD_HEIGHT + 1) * BLOCK_SIZE;
    int INFOR_WIDTH      = 5;
    
    hold  = new Hold(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map   = new Map(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next  = new LinkListBlock(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    next->setSeed(0);
    infor = new Infor(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * BLOCK_SIZE);
    
    collision = false;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}