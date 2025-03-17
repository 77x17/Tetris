#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

#include <iostream>

Monitor::Monitor(int x, int y) : X_COORDINATE(x), Y_COORDINATE(y) {
    gameOver = false;

    int HOLD_WIDTH       = 5;
    int HOLD_HEIGHT      = 3;
    int HOLD_POSITION_X  = X_COORDINATE;
    int HOLD_POSITION_Y  = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int GRID_WIDTH       = 10;
    int GRID_HEIGHT      = 24;
    int GRID_POSITION_X  = HOLD_POSITION_X + BLOCK_SIZE * (HOLD_WIDTH + 1);
    int GRID_POSITION_Y  = Y_COORDINATE;
    
    int NEXT_WIDTH       = 5;
    int NEXT_HEIGHT      = 15;
    int NEXT_POSITION_X  = GRID_POSITION_X + BLOCK_SIZE * (GRID_WIDTH + 1);
    int NEXT_POSITION_Y  = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int INFOR_POSITION_X = HOLD_POSITION_X;
    int INFOR_POSITION_Y = HOLD_POSITION_Y + (HOLD_HEIGHT + 1) * BLOCK_SIZE;
    int INFOR_WIDTH      = 5;
    
    hold      = new Hold(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map       = new Map(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next      = new LinkListBlock(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    infor     = new Infor(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * BLOCK_SIZE);
}

Monitor::~Monitor() {
    delete hold;     hold     = nullptr;
    delete next;     next     = nullptr;
    delete map;      map      = nullptr;
    delete infor;    infor    = nullptr;
}

bool Monitor::isGameOver() {
    return gameOver;
}

void Monitor::draw(sf::RenderWindow* window) {
    map ->drawOutline(window);
    hold->drawOutline(window);
    next->drawOutline(window);
    hold    ->draw(window);
    next    ->draw(window);
    map     ->draw(window);
    infor   ->draw(window);
}

void Monitor::clearScreen(uint32_t seed) {
    hold->reset();
    map->reset();
    next->reset(seed);
    infor->reset();
}