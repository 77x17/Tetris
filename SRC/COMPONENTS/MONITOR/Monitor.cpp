#include "Monitor.hpp"

#include "Common.hpp"
#include "Hold.hpp"
#include "LinkListBlock.hpp"
#include "Block.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

#include <iostream>

Monitor::Monitor(int x, int y) {
    X_COORDINATE = x; Y_COORDINATE = y;
    gameOver = false;
}

Monitor::~Monitor() {
    delete hold; hold = nullptr;
    delete next; next = nullptr;
}

void Monitor::setGameOver() { gameOver = true; }

bool Monitor::isGameOver() { return gameOver; }

void Monitor::draw(sf::RenderWindow* window, CurrentBlock* block) const {
    hold->drawOutline(window);
    next->drawOutline(window);
    hold    ->draw(window);
    next    ->draw(window);
}

void Monitor::resetMonitor(uint32_t seed) {
    hold ->reset();
    // map  ->reset();
    next ->reset(seed);
    // infor->reset();
    gameOver = false;
}

bool Monitor::canHold() { return hold->canHold(); }
void Monitor::unlockHold() { hold->unlock(); }
void Monitor::lockHold() { hold->lock(); }
void Monitor::exchangeCurrentBlock(CurrentBlock* curBlock) {
    lockHold();
    curBlock->swap(hold);
    if (curBlock->isEmpty()) {
        curBlock->setter(next->updateNext());
    } // inheritate and curBocl will reset position from child class
}

Block* Monitor::getNext() const {
    return next->updateNext();
}