#include "MovementController_VersusBot.hpp"

#include "Bot.hpp"
#include "Monitor_Multiplayer.hpp"
#include "CurrentBlockController.hpp"
#include "CurrentBlock_Multiplayer.hpp"

MovementController_VersusBot::MovementController_VersusBot(Monitor* monitor, CurrentBlockController* controller)
                            : MovementController(monitor, controller){}

MovementController_VersusBot::~MovementController_VersusBot(){}

void MovementController_VersusBot::handlePut() {
    int nLinesRemove = monitor->putIntoMap(curBlock->getCurrentBlock());
    if (nLinesRemove == 0) {
        std::random_device rd; std::mt19937 gen(rd());
        dynamic_cast<Monitor_Multiplayer*>(monitor)->mapReceiveLineFromCompetitor(gen());
    }

    nLinesRemove = monitor->removeNLines(nLinesRemove, curBlock->getCurrentBlock(), curBlock->isAllClear());

    if (nLinesRemove > 0) {
        dynamic_cast<Monitor_Multiplayer*>(competitor)->inforReceiveLineFromCompetitor(nLinesRemove);
    }

    collision = false;
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
    if (curBlock->gameOver())
        monitor->setGameOver();
}

void MovementController_VersusBot::setCompetitor(Monitor* monitor) {
    competitor = monitor;
}