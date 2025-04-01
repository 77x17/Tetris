#include "Monitor_Singleplayer.hpp"

#include "Common.hpp"
#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"
#include "SoundManager.hpp"

#include <iostream>

Monitor_Singleplayer::Monitor_Singleplayer(int x, int y):Monitor(x, y) {
    CreateMonitor(x, y);
}

void Monitor_Singleplayer::CreateMonitor(int x, int y) {
    map          = new Map();
    infor        = new Infor();
    hold         = new Hold();
    next         = new LinkListBlock();

    setPosition(x, y);
}

void Monitor_Singleplayer::setPosition(int X_COORDINATE, int Y_COORDINATE) {
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

    hold        ->setPosition(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map         ->setPosition(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next        ->setPosition(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    infor       ->setPosition(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * Common::BLOCK_SIZE);
    soundManager->setPosition(AUDIO_POSITION_X, AUDIO_POSITION_Y, AUDIO_WIDTH, AUDIO_HEIGHT);
}

void Monitor_Singleplayer::exchangeCurrentBlock(CurrentBlock* curBlock) {
    Monitor::exchangeCurrentBlock(curBlock);
    curBlock->resetPosition(map);
}

void Monitor_Singleplayer::draw(sf::RenderWindow* window, CurrentBlock* block) const {
    map->drawOutline(window);
    map->draw(window);
    map->drawCurrentBlock(window, block);
    infor->draw(window);
    Monitor::draw(window, block);
}

void Monitor_Singleplayer::resetMonitor(uint32_t seed) {
    Monitor::resetMonitor(seed);
    map  ->reset();
    infor->reset();
}

uint8_t Monitor_Singleplayer::removeNLines(int nLines, CurrentBlock* curBlock, bool isAllClear) {
    infor->update(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock(), isAllClear);
    infor->playSound(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock(), isAllClear);
    return infor->removeLine(nLines);
}

Map* Monitor_Singleplayer::getMap() const {
    return map;
}

int Monitor_Singleplayer::putIntoMap(CurrentBlock* curBlock) {
    return curBlock->putIntoMap(map);
}

void Monitor_Singleplayer::setTimer() {
    infor->setTimer();
}

void Monitor_Singleplayer::pauseTimer() {
    infor->pauseTimer();
}

void Monitor_Singleplayer::unPauseTimer() {
    infor->unPauseTimer();
}