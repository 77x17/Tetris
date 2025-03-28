#include "MovementControllerWithBot.hpp"

#include "Bot.hpp"
#include "MonitorForTwoPlayer.hpp"
#include "CurrentBlockController.hpp"
#include "CurrentBlockWithNetwork.hpp"

MovementControllerWithBot::MovementControllerWithBot(Monitor* monitor, CurrentBlockController* controller)
                            : MovementController(monitor, controller){}

MovementControllerWithBot::~MovementControllerWithBot(){}

void MovementControllerWithBot::handlePut() {
    int nLinesRemove = monitor->putIntoMap(curBlock->getCurrentBlock());
    if (nLinesRemove == 0) {
        std::random_device rd; std::mt19937 gen(rd());
        dynamic_cast<MonitorForTwoPlayer*>(monitor)->mapReceiveLineFromCompetitor(gen());
    }

    nLinesRemove = monitor->removeNLines(nLinesRemove, curBlock->getCurrentBlock());

    if (nLinesRemove > 0) {
        competitor->handleAddLine(nLinesRemove);
    }

    collision = false;
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
    if (curBlock->gameOver())
        monitor->setGameOver();
}

void MovementControllerWithBot::setCompetitor(Bot* b) {
    competitor = b;
}