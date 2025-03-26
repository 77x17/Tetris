#include "Monitor.hpp"

#include "Common.hpp"
#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

#include <iostream>

Monitor::Monitor() { 
    gameOver = false;
    hold     = nullptr;
    next     = nullptr;
    map      = nullptr;
    infor    = nullptr;
}

Monitor::~Monitor() {
    delete hold;     hold     = nullptr;
    delete next;     next     = nullptr;
    delete map;      map      = nullptr;
    delete infor;    infor    = nullptr;
}

void Monitor::setGameOver() { gameOver = true; }

bool Monitor::isGameOver() { return gameOver; }

void Monitor::draw(sf::RenderWindow* window, CurrentBlock* block) const {
    map ->drawOutline(window);
    hold->drawOutline(window);
    next->drawOutline(window);

    hold    ->draw(window);
    next    ->draw(window);
    map     ->draw(window);
    infor   ->draw(window);

    map->drawCurrentBlock(window, block);
}

void Monitor::resetMonitor(uint32_t seed) {
    hold ->reset();
    map  ->reset();
    next ->reset(seed);
    infor->reset();
    gameOver = false;
}

void Monitor::setNewSeed(int seed) {
    next->reset(seed);
}

uint8_t Monitor::addLineToInfor(int nLines, CurrentBlock* curBlock) {
    infor->update(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    infor->playSound(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    return infor->removeLine(nLines);
}

Map* Monitor::getMap() const {
    return map;
}

bool Monitor::canHold() { return hold->canHold(); }
void Monitor::unlockHold() { hold->unlock(); }
void Monitor::lockHold() { hold->lock(); }
void Monitor::exchangeCurrentBlock(CurrentBlock* curBlock) {
    lockHold();
    curBlock->swap(hold);
    if (curBlock->isEmpty()) {
        curBlock->setter(next->updateNext());
    }
    curBlock->resetPosition(map);
}

LinkListBlock* Monitor::getNext() const {
    return next;
}

Infor* Monitor::getInfor() const {
    return infor;
}