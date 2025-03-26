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
}

void CurrentBlockController::resetPosition() {
    block->setState(2, WIDTH_MAP / 2 - BLOCK_EDGE / 2, 0, 0);
    updateShadow();
}

bool CurrentBlockController::fallDown() {
    if (not map->isValid((block->block)->getShape(), block->posY + 1, block->posX)) 
        return false;

    block->posY++;

    return true;
}

void CurrentBlockController::updateShadow() {
    int8_t &shadow = block->shadowPosY;
    int8_t &posX   = block->posX;

    uint16_t shape = (block->block)->getShape();
    shadow = block->posY;
    while (map->isValid(shape, shadow + 1, posX)) {
        shadow++;
    }
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
    block->posX--; updateShadow();
    soundManager->play("move");
    return true;
}

bool CurrentBlockController::moveRight() {
    if (not map->isValid((block->block)->getShape(), block->posY, block->posX + 1)) 
        return false; 
    block->posX++; updateShadow();
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

bool CurrentBlockController::rotateLeft() {
    Block*& cur = block->block;
    int8_t &posY = block->posY;
    int8_t &posX = block->posX;

    if (not isValid(cur->getRotateLeft())) {
        posY++;
        if (isValid(cur->getRotateLeft())) goto rotate_success;
        posY--;
        
        posY--;
        if (posY >= 0 and isValid(cur->getRotateLeft())) goto rotate_success;
        posY++;

        posX++;
        if (isValid(cur->getRotateLeft())) goto rotate_success;
        posX--;

        posX--;
        if (posX >= 0 and isValid(cur->getRotateLeft())) goto rotate_success;
        posX++;        

        if (cur->getShapeID() == 0) {
            posY += 2;
            if (isValid(cur->getRotateLeft())) goto rotate_success;
            if (isValid(cur->getRotateRight())) goto rotate_success;
            posY -= 2;

            posY -= 2;
            if (posY >= 0 and isValid(cur->getRotateLeft())) goto rotate_success;
            if (posY >= 0 and isValid(cur->getRotateRight())) goto rotate_success;
            posY += 2;
            
            posX += 2;
            if (isValid(cur->getRotateLeft())) goto rotate_success;
            if (isValid(cur->getRotateRight())) goto rotate_success;
            posX -= 2;

            posX -= 2;
            if (posX >= 0 and isValid(cur->getRotateLeft())) goto rotate_success;
            if (posX >= 0 and isValid(cur->getRotateRight())) goto rotate_success;
            posX += 2;
        }

        return false;
    }

rotate_success:
    cur->rotateLeft();

    updateShadow();

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

bool CurrentBlockController::rotateRight() {

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

char CurrentBlockController::getTypeBlock() {
    switch ((block->block)->getShapeID()) {
        case 1:
            return 'I';
        case 2:
            return 'L';
        case 3:
            return 'J';
        case 4:
            return 'O';
        case 5:
            return 'Z';
        case 6:
            return 'S';
        case 7:
            return 'T';
        default:
            throw std::invalid_argument("Invalid getTypeBlock()");
    }

    return char();
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
    packet << (block->block)->getStateID() << block->posY << block->posX << block->shadowPosY << isJustSpin() << getTypeBlock();
}