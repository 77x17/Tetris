#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

constexpr float DROP_TIME = 0.5f;
constexpr float COLLISION_DROP_TIME = 1.0f;

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

void Monitor::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } 
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left and curBlock->moveLeft(map)) {
                if (curBlock->collisionBottom(map)) {
                    collision = true;
                    collisionClock.restart();
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Right and curBlock->moveRight(map)) {
                if (curBlock->collisionBottom(map)) {
                    collision = true;
                    collisionClock.restart();
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Down and curBlock->moveDown(map)) {
                if (not curBlock->collisionBottom(map)) {
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Up and curBlock->rotateLeft(map)) {
                if (curBlock->collisionBottom(map)) {
                    collision = true;
                    collisionClock.restart();
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Space) {
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
            else if (event.key.code == sf::Keyboard::C and hold->canHold()) {
                hold->lock();
                curBlock->swap(hold);
                if (curBlock->isEmpty()) {
                    curBlock->setter(next->updateNext());
                }
                curBlock->resetPosition(map);

                clock.restart();
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