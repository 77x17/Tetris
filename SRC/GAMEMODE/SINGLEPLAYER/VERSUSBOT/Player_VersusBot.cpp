#include "Player_VersusBot.hpp"

#include "CurrentBlockController.hpp"
#include "Monitor_Multiplayer.hpp"
#include "MovementController_VersusBot.hpp"
#include "Bot.hpp"

#include <iostream>
#include <random>
#include <SFML/Network.hpp>

Player_VersusBot::Player_VersusBot(int x, int y): Player(x, y) {
    initialize();
}

void Player_VersusBot::initialize() {
    monitor = new Monitor_Multiplayer(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlockController(monitor->getMap());
    movementController = new MovementController_VersusBot(monitor, curBlock);
}

Player_VersusBot::~Player_VersusBot() {}

void Player_VersusBot::setCompetitor(Monitor* mon) {
    dynamic_cast<MovementController_VersusBot*>(movementController)->setCompetitor(mon);
}

void Player_VersusBot::setGameOver() {
    throw std::runtime_error("Not install set gameover yet");
    // dynamic_cast<MovementController_VersusBot*>(movementController)->setGameOver();
}

void Player_VersusBot::start(uint32_t seed, Bot* b) {
    b->setCompetitor(monitor); // set monitor for other monitor;

    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
}