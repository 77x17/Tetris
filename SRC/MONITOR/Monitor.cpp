#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

#include <iostream>

Monitor::Monitor(int x, int y) : X_COORDINATE(x), Y_COORDINATE(y) {
    gameOver = false;

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

    int GARBAGE_WIDTH = 1;
    int GARBAGE_HEIGHT = GRID_HEIGHT;
    int GARBAGE_POSITION_X = GRID_POSITION_X - BLOCK_SIZE;
    int GARBAGE_POSITION_Y = Y_COORDINATE + GRID_HEIGHT * BLOCK_SIZE;

    hold      = new Hold         (   HOLD_POSITION_X,    HOLD_POSITION_Y,    HOLD_WIDTH             ,    HOLD_HEIGHT);
    map       = new Map          (   GRID_POSITION_X,    GRID_POSITION_Y,    GRID_WIDTH             ,    GRID_HEIGHT);
    next      = new LinkListBlock(   NEXT_POSITION_X,    NEXT_POSITION_Y,    NEXT_WIDTH             ,    NEXT_HEIGHT);
    infor     = new Infor        (  INFOR_POSITION_X,   INFOR_POSITION_Y,   INFOR_WIDTH * BLOCK_SIZE,
                                    AUDIO_POSITION_X,   AUDIO_POSITION_Y,   AUDIO_WIDTH             ,   AUDIO_HEIGHT,
                                  GARBAGE_POSITION_X, GARBAGE_POSITION_Y, GARBAGE_WIDTH             , GARBAGE_HEIGHT);
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
    hold ->reset();
    map  ->reset();
    next ->reset(seed);
    infor->reset();
}