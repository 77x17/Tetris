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

Monitor::Monitor(sf::RenderWindow* newWindow, int x, int y) : X_COORDINATE(x), Y_COORDINATE(y) {
    window = newWindow;
    int HOLD_WIDTH      = 5;
    int HOLD_HEIGHT     = 3;
    int HOLD_POSITION_X = X_COORDINATE;
    int HOLD_POSITION_Y = Y_COORDINATE + 5 * BLOCK_SIZE;
    hold = new Hold(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    
    int GRID_WIDTH      = 10;
    int GRID_HEIGHT     = 24;
    int GRID_POSITION_X = HOLD_POSITION_X + BLOCK_SIZE * (HOLD_WIDTH + 1);
    int GRID_POSITION_Y = Y_COORDINATE;
    map = new Map(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    
    int NEXT_WIDTH      = 5;
    int NEXT_HEIGHT     = 15;
    int NEXT_POSITION_X = GRID_POSITION_X + BLOCK_SIZE * (GRID_WIDTH + 1);
    int NEXT_POSITION_Y = Y_COORDINATE + 5 * BLOCK_SIZE;
    next = new LinkListBlock(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    
    infor = new Infor();

    curBlock  = new CurrentBlock();
    collision = false;
}

Monitor::~Monitor() {
    if (curBlock) {delete curBlock; curBlock = nullptr;}
    if (hold) {delete hold; hold = nullptr;}
    if (next) {delete next; next = nullptr;}
    if (map) {delete map; map = nullptr;}
    if (infor) {delete infor; infor = nullptr;}
}

void Monitor::handleLeft() {
    if (curBlock->moveLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
        collisionClock.restart();
    }
}

void Monitor::handleRight() {
    if (curBlock->moveRight(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
        collisionClock.restart();
    }
}

void Monitor::handleDown() {
    if (curBlock->moveDown(map) and not curBlock->collisionBottom(map)) {
        collision = true;

        clock.restart();
    }
}

void Monitor::handleUp() {
    if (curBlock->rotateLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;

        clock.restart();
        collisionClock.restart();
    }
}

void Monitor::handleHardDrop() {
    curBlock->hardDrop(map); 

    curBlock->put(map);
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map); 

    if (curBlock->gameOver(map)) {
        restart();
    }

    hold->unlock();

    clock.restart();
    collisionClock.restart();
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
        collisionClock.restart();
    }
}

bool moveLeft  = false;
bool moveRight = false;
bool moveDown  = false;

void Monitor::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } 
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left and moveLeft == false) {
                moveLeft  = true;
                moveRight = false;

                handleLeft();
                
                movingTime = DELAY_MOVING_TIME;

                movingClock.restart();
            }
            else if (event.key.code == sf::Keyboard::Right and moveRight == false) {
                moveLeft  = false;
                moveRight = true;

                handleRight();

                movingTime = DELAY_MOVING_TIME;

                movingClock.restart();
            } 
            else if (event.key.code == sf::Keyboard::Down and moveDown == false) {
                moveDown = true;

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
                moveLeft = false;
                
                movingTime = DELAY_MOVING_TIME;
            }
            else if (event.key.code == sf::Keyboard::Right) {
                moveRight = false;
                
                movingTime = DELAY_MOVING_TIME;
            } 
            else if (event.key.code == sf::Keyboard::Down) {
                moveDown = false;
            } 
        }
    }
}

void Monitor::update() {
    if (movingClock.getElapsedTime().asMilliseconds() >= movingTime) {
        if (moveLeft) {
            handleLeft();
            movingTime = MOVING_TIME;
        }
        else if (moveRight) {
            handleRight();
            movingTime = MOVING_TIME;
        }
        
        if (moveDown) {
            handleDown();
            movingTime = MOVING_TIME;
        }
        
        movingClock.restart();
    }

    if (collision) {
        if (collisionClock.getElapsedTime().asSeconds() >= COLLISION_DROP_TIME) {
            if (not curBlock->moveDown(map)) {
                curBlock->put(map);
    
                curBlock->setter(next->updateNext());
                curBlock->resetPosition(map);
                
                if (curBlock->gameOver(map)) {
                    restart();
                }
                
                hold->unlock();
            }
            
            collisionClock.restart();

            collision = false;
        }
    }
    else {
        if (clock.getElapsedTime().asSeconds() >= DROP_TIME) {
            if (not curBlock->moveDown(map)) {
                curBlock->put(map);

                curBlock->setter(next->updateNext());
                curBlock->resetPosition(map);
                
                if (curBlock->gameOver(map)) {
                    restart();
                }
                
                hold->unlock();
            }
            
            clock.restart();
        }
    }
}

void Monitor::render() {
    window->clear();

    map ->drawOutline(window);
    hold->drawOutline(window);
    next->drawOutline(window);
    curBlock->draw(window, map);
    hold    ->draw(window);
    next    ->draw(window);
    map     ->draw(window);
     
    window->display();
}

void Monitor::restart() {
    int HOLD_WIDTH      = 5;
    int HOLD_HEIGHT     = 3;
    int HOLD_POSITION_X = X_COORDINATE;
    int HOLD_POSITION_Y = Y_COORDINATE + 5 * BLOCK_SIZE;
    delete hold;
    hold = new Hold(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);

    int GRID_WIDTH      = 10;
    int GRID_HEIGHT     = 24;
    int GRID_POSITION_X = HOLD_POSITION_X + BLOCK_SIZE * (HOLD_WIDTH + 1);
    int GRID_POSITION_Y = Y_COORDINATE;
    delete map;
    map = new Map(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    
    int NEXT_WIDTH      = 5;
    int NEXT_HEIGHT     = 15;
    int NEXT_POSITION_X = GRID_POSITION_X + BLOCK_SIZE * (GRID_WIDTH + 1);
    int NEXT_POSITION_Y = Y_COORDINATE + 5 * BLOCK_SIZE;
    delete next;
    next = new LinkListBlock(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    
    delete infor;
    infor = new Infor();
    
    collision = false;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}