#include "CurrentBlockController.hpp"

#include <iostream>

#include "Common.hpp"
#include "CurrentBlock.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "Block.hpp"
#include "SoundManager.hpp"

CurrentBlockController::CurrentBlockController(Map* map) : map(map) {
    soundManager = new SoundManager();
    block = new CurrentBlock();
    soundManager->loadSound("hardDrop", "ASSETS/sfx/harddrop.mp3");
    soundManager->loadSound("hold"    , "ASSETS/sfx/hold.mp3");
    soundManager->loadSound("move"    , "ASSETS/sfx/move.mp3");
    soundManager->loadSound("rotate"  , "ASSETS/sfx/rotate.mp3");
    soundManager->loadSound("spin"    , "ASSETS/sfx/spin.mp3");
    soundManager->loadSound("topout"  , "ASSETS/sfx/topout.mp3");
}

// CurrentBlockController::CurrentBlockController(CurrentBlock *a) : block(a){ block = nullptr; }

CurrentBlockController::~CurrentBlockController() {
    delete block; block = nullptr;
    delete soundManager;
}

bool CurrentBlockController::isEmpty() { return block->isEmpty(); }

CurrentBlock* CurrentBlockController::getCurrentBlock() const { return block; }
bool CurrentBlockController::isJustSpin() { return block->isJustSpin(); };

void CurrentBlockController::setter(Block* p) {
    block->freeAndSetter(p);
    block->resetPosition(map);
}

bool CurrentBlockController::fallDown() {
    if (not map->isValid((block->block)->getShape(), block->posY + 1, block->posX)) 
        return false;

    block->posY++;

    return true;
}

bool CurrentBlockController::moveDown() {
    if (not map->isValid((block->block)->getShape(), block->posY + 1, block->posX)) 
        return false; 
    block->posY++;
    soundManager->play("move");
    return true;
}

bool CurrentBlockController::moveLeft() {
    if (not map->isValid((block->block)->getShape(), block->posY, block->posX - 1)) 
        return false; 
    block->posX--; block->updateShadow(map);
    soundManager->play("move");
    return true;
}

bool CurrentBlockController::moveRight() {
    if (not map->isValid((block->block)->getShape(), block->posY, block->posX + 1)) 
        return false; 
    block->posX++; block->updateShadow(map);
    soundManager->play("move");
    return true;
}

bool CurrentBlockController::hardDrop() {
    block->posY = block->shadowPosY;
    soundManager->play("hardDrop");
    return true;
}

bool CurrentBlockController::collisionBottom() {
    return not map->isValid((block->block)->getShape(), block->posY+1, block->posX);
}

bool CurrentBlockController::rotate(int8_t times) {
    Block*& cur  = block->block;
    int8_t &posY = block->posY;
    int8_t &posX = block->posX;

    if (not isValid(cur->getRotate(times))) {
        posY++;
        if (isValid(cur->getRotate(times))) goto rotate_success;
        posY--;
        
        posY--;
        if (posY >= 0 and isValid(cur->getRotate(times))) goto rotate_success;
        posY++;

        posX++;
        if (isValid(cur->getRotate(times))) goto rotate_success;
        posX--;

        posX--;
        if (posX >= 0 and isValid(cur->getRotate(times))) goto rotate_success;
        posX++;        

        if (cur->getShapeID() == 1) {
            posY += 2;
            if (isValid(cur->getRotate(times)))  goto rotate_success;
            if (isValid(cur->getRotate(3 - times))) goto rotate_success;
            posY -= 2;

            posY -= 2;
            if (posY >= 0 and isValid(cur->getRotate(times)))  goto rotate_success;
            if (posY >= 0 and isValid(cur->getRotate(3 - times))) goto rotate_success;
            posY += 2;
            
            posX += 2;
            if (isValid(cur->getRotate(times)))  goto rotate_success;
            if (isValid(cur->getRotate(3-times))) goto rotate_success;
            posX -= 2;

            posX -= 2;
            if (posX >= 0 and isValid(cur->getRotate(times)))  goto rotate_success;
            if (posX >= 0 and isValid(cur->getRotate(3 - times))) goto rotate_success;
            posX += 2;
        }

        return false;
    }
    rotate_success:
        cur->rotate(times);
    
        block->updateShadow(map);
    
        if (posY != 0 and not map->isValid(cur->getShape(), posY - 1, posX)) {
            block->setSpin();
            soundManager->play("spin");
        }
        else {
            block->resetSpin();
            soundManager->play("rotate");
        }
    return true;
}

bool CurrentBlockController::rotateLeft() {
    rotate(1);
    return true;
}

bool CurrentBlockController::rotateRight() {
    rotate(3);
    return true;
}

bool CurrentBlockController::rotate180() {
    rotate(2);
    return true;
}

void CurrentBlockController::swap(Hold* hold) {
    block->swap(hold);
    soundManager->play("hold");
}

void CurrentBlockController::draw(sf::RenderWindow *window) {
    map->drawCurrentBlock(window, block);
}

bool CurrentBlockController::isValid(uint16_t shape) {
    return map->isValid(shape, block->posY, block->posX);
}

uint8_t CurrentBlockController::putIntoMap() {
    return block->putIntoMap(map);
}

bool CurrentBlockController::gameOver() {
    bool isGameOver = !block->isValid(map);
    if (isGameOver) {
        soundManager->play("topout");
    }
    return isGameOver;
}

void CurrentBlockController::compress(sf::Packet &packet) {
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY;
}

void CurrentBlockController::compressWithSpin(sf::Packet &packet) {
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY << isJustSpin();
}