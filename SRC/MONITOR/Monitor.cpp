#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

Monitor::Monitor(sf::RenderWindow* newWindow) {
    window   = newWindow;
    map      = new Map();
    hold     = new Hold();
    next     = new LinkListBlock();
    curBlock = new CurrentBlock();
    infor    = new Infor();
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
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left and curBlock->moveLeft(map)) {
                if (curBlock->collisionBottom(map)) {
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Right and curBlock->moveRight(map)) {
                if (curBlock->collisionBottom(map)) {
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
                    clock.restart();
                }
            } else if (event.key.code == sf::Keyboard::Space) {
                curBlock->hardDrop(map); 

                curBlock->put(map);
                curBlock->setter(next->updateNext());
                curBlock->resetPosition(map); // Check gameover

                hold->unlock();

                clock.restart();
            } else if (event.key.code == sf::Keyboard::C and hold->canHold()) {
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
