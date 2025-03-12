#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

constexpr float DROP_TIME = 0.5f;
constexpr float COLLISION_DROP_TIME = 2.5f;

Monitor::Monitor(sf::RenderWindow* newWindow) {
    window    = newWindow;
    map       = new Map();
    hold      = new Hold();
    next      = new LinkListBlock();
    curBlock  = new CurrentBlock();
    infor     = new Infor();
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
        collision = true;
        clock.restart();
    }
}

void Monitor::handleUp() {
    if (curBlock->rotateLeft(map) and curBlock->collisionBottom(map) and collision == false) {
        collision = true;
        clock.restart();
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

void Monitor::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } 
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                handleLeft();
            }
            else if (event.key.code == sf::Keyboard::Right) {
                handleRight();
            } 
            else if (event.key.code == sf::Keyboard::Down) {
                handleDown();
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
    }
}

void Monitor::update() {
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
    curBlock->draw(window);
    hold    ->draw(window);
    next    ->draw(window);
    map     ->draw(window);
     
    window->display();
}

void Monitor::restart() {
    delete map;
    map = new Map();
    delete hold;
    hold = new Hold();
    delete next;
    next = new LinkListBlock();

    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}