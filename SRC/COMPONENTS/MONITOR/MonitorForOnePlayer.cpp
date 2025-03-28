#include "MonitorForOnePlayer.hpp"

#include "Common.hpp"
#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"
#include "SoundManager.hpp"

#include <iostream>

MonitorForOnePlayer::MonitorForOnePlayer(int x, int y):Monitor(x, y) {
    CreateMonitor(x, y);
}

void MonitorForOnePlayer::CreateMonitor(int x, int y) {
    map          = new Map();
    infor        = new Infor();
    hold         = new Hold();
    next         = new LinkListBlock();

    setPosition(x, y);
}

void MonitorForOnePlayer::setPosition(int X_COORDINATE, int Y_COORDINATE) {
    int HOLD_WIDTH         = 5;
    int HOLD_HEIGHT        = 3;
    int HOLD_POSITION_X    = X_COORDINATE;
    int HOLD_POSITION_Y    = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int GRID_WIDTH         = 10;
    int GRID_HEIGHT        = 24;
    int GRID_POSITION_X    = HOLD_POSITION_X + HOLD_WIDTH * BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE;
    int GRID_POSITION_Y    = Y_COORDINATE;
    
    int NEXT_WIDTH         = 5;
    int NEXT_HEIGHT        = 15;
    int NEXT_POSITION_X    = GRID_POSITION_X + GRID_WIDTH * BLOCK_SIZE + BLOCK_SIZE;
    int NEXT_POSITION_Y    = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int INFOR_WIDTH        = 5;
    int INFOR_POSITION_X   = HOLD_POSITION_X;
    int INFOR_POSITION_Y   = HOLD_POSITION_Y + HOLD_HEIGHT * BLOCK_SIZE + BLOCK_SIZE;

    int AUDIO_WIDTH        = 5;
    int AUDIO_HEIGHT       = 1;
    int AUDIO_POSITION_X   = GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE + BLOCK_SIZE;
    int AUDIO_POSITION_Y   = GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE - BLOCK_SIZE;

    hold        ->setPosition(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map         ->setPosition(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next        ->setPosition(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    infor       ->setPosition(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * BLOCK_SIZE);
    soundManager->setPosition(AUDIO_POSITION_X, AUDIO_POSITION_Y, AUDIO_WIDTH, AUDIO_HEIGHT);
}

void MonitorForOnePlayer::exchangeCurrentBlock(CurrentBlock* curBlock) {
    Monitor::exchangeCurrentBlock(curBlock);
    curBlock->resetPosition(map);
}

void MonitorForOnePlayer::draw(sf::RenderWindow* window, CurrentBlock* block) const {
    map->drawOutline(window);
    map->draw(window);
    map->drawCurrentBlock(window, block);
    infor->draw(window);
    Monitor::draw(window, block);
}

void MonitorForOnePlayer::resetMonitor(uint32_t seed) {
    Monitor::resetMonitor(seed);
    map  ->reset();
    infor->reset();
}

uint8_t MonitorForOnePlayer::removeNLines(int nLines, CurrentBlock* curBlock) {
    infor->update(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    infor->playSound(nLines, curBlock->isJustSpin(), curBlock->getTypeBlock());
    return infor->removeLine(nLines);
}

Map* MonitorForOnePlayer::getMap() const {
    return map;
}

int MonitorForOnePlayer::putIntoMap(CurrentBlock* curBlock) {
    return curBlock->putIntoMap(map);
}

void MonitorForOnePlayer::setTimer() {
    infor->setTimer();
}

void MonitorForOnePlayer::pauseTimer() {
    infor->pauseTimer();
}

void MonitorForOnePlayer::unPauseTimer() {
    infor->unPauseTimer();
}