#include "Monitor_Multiplayer.hpp"

#include "Common.hpp"
#include "Hold.hpp"
#include "Map_Multiplayer.hpp"
#include "Infor_Multiplayer.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"
#include "SoundManager.hpp"

#include <iostream>

Monitor_Multiplayer::Monitor_Multiplayer(int x, int y):Monitor(x, y) {}

void Monitor_Multiplayer::createMonitor(int x, int y) {
    hold = new Hold();
    map  = new Map_Multiplayer();
    next = new LinkListBlock();
    infor = new Infor_Multiplayer();

    setPosition(x, y);
}

Monitor_Multiplayer::~Monitor_Multiplayer() {
    delete map;
    delete infor;
}

void Monitor_Multiplayer::setPosition(int X_COORDINATE, int Y_COORDINATE) {
    int HOLD_WIDTH         = 5;
    int HOLD_HEIGHT        = 3;
    int HOLD_POSITION_X    = X_COORDINATE;
    int HOLD_POSITION_Y    = Y_COORDINATE + 5 * Common::BLOCK_SIZE;
    
    int GRID_WIDTH         = 10;
    int GRID_HEIGHT        = 24;
    int GRID_POSITION_X    = HOLD_POSITION_X + HOLD_WIDTH * Common::BLOCK_SIZE + Common::BLOCK_SIZE + Common::BLOCK_SIZE;
    int GRID_POSITION_Y    = Y_COORDINATE;
    
    int NEXT_WIDTH         = 5;
    int NEXT_HEIGHT        = 15;
    int NEXT_POSITION_X    = GRID_POSITION_X + GRID_WIDTH * Common::BLOCK_SIZE + Common::BLOCK_SIZE;
    int NEXT_POSITION_Y    = Y_COORDINATE + 5 * Common::BLOCK_SIZE;
    
    int INFOR_WIDTH        = 5;
    int INFOR_POSITION_X   = HOLD_POSITION_X;
    int INFOR_POSITION_Y   = HOLD_POSITION_Y + HOLD_HEIGHT * Common::BLOCK_SIZE + Common::BLOCK_SIZE;

    int AUDIO_WIDTH        = 5;
    int AUDIO_HEIGHT       = 1;
    int AUDIO_POSITION_X   = GRID_POSITION_X + GRID_WIDTH  * Common::BLOCK_SIZE + Common::BLOCK_SIZE;
    int AUDIO_POSITION_Y   = GRID_POSITION_Y + GRID_HEIGHT * Common::BLOCK_SIZE - Common::BLOCK_SIZE;

    int GARBAGE_WIDTH = 1;
    int GARBAGE_HEIGHT = GRID_HEIGHT;
    int GARBAGE_POSITION_X = GRID_POSITION_X - Common::BLOCK_SIZE;
    int GARBAGE_POSITION_Y = Y_COORDINATE + GRID_HEIGHT * Common::BLOCK_SIZE;

    hold->setPosition(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map->setPosition(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next->setPosition(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    infor->setPosition(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * Common::BLOCK_SIZE, GARBAGE_POSITION_X, GARBAGE_POSITION_Y, GARBAGE_WIDTH, GARBAGE_HEIGHT);
    soundManager->setPosition(AUDIO_POSITION_X, AUDIO_POSITION_Y, AUDIO_WIDTH, AUDIO_HEIGHT);
}

void Monitor_Multiplayer::setNewSeed(int seed) {
    next->reset(seed);
}

void Monitor_Multiplayer::exchangeCurrentBlock(CurrentBlock* curBlock) {
    Monitor::exchangeCurrentBlock(curBlock);
    curBlock->resetPosition(map);
}

void Monitor_Multiplayer::draw(sf::RenderWindow* window, CurrentBlock* block) const {
    map->drawOutline(window);
    map->draw(window);
    map->drawCurrentBlock(window, block);
    infor->draw(window, 1);
    Monitor::draw(window, block);
}

void Monitor_Multiplayer::resetMonitor(uint32_t seed) {
    Monitor::resetMonitor(seed);
    map  ->reset();
    infor->reset();
}

uint8_t Monitor_Multiplayer::removeNLines(int nLines, CurrentBlock* curBlock, bool isAllClear) {
    infor->update(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock(), isAllClear);
    infor->playSound(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock(), isAllClear);
    return infor->removeLine(nLines);
}

void Monitor_Multiplayer::inforReceiveLineFromCompetitor(int nLines) {
    infor->addLine(nLines);
}

void Monitor_Multiplayer::mapReceiveLineFromCompetitor(int seed) {
    map->add(infor->getAndRemoveLineAdd(), seed);
}

Map* Monitor_Multiplayer::getMap() const {
    return map;
}

int Monitor_Multiplayer::putIntoMap(CurrentBlock* curBlock) {
    return curBlock->putIntoMap(map);
}

void Monitor_Multiplayer::setTimer() {
    infor->setTimer();
}

void Monitor_Multiplayer::pauseTimer() {
    infor->pauseTimer();
}

void Monitor_Multiplayer::unPauseTimer() {
    infor->unPauseTimer();
}