#include "MovementControllerWithBot.hpp"

#include "MonitorForTwoPlayer.hpp"
#include "Infor.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "LinkListBlock.hpp"
#include "CurrentBlockController.hpp"
#include "CurrentBlockWithNetwork.hpp"

MovementControllerWithBot::MovementControllerWithBot(Monitor* monitor, CurrentBlockController* controller)
                            : MovementController(monitor, controller){}

MovementControllerWithBot::~MovementControllerWithBot(){}

void MovementControllerWithBot::handlePut() {
    int nLinesRemove = monitor->putIntoMap(curBlock->getCurrentBlock());

    collision = false;
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
    if (curBlock->gameOver())
        monitor->setGameOver();

    if (nLinesRemove == 0) {
        std::random_device rd; std::mt19937 gen(rd());
        dynamic_cast<MonitorForTwoPlayer*>(monitor)->mapReceiveLineFromCompetitor(gen());
    }

    nLinesRemove = monitor->removeNLines(nLinesRemove, curBlock->getCurrentBlock());

    if (nLinesRemove > 0) {
        dynamic_cast<MonitorForTwoPlayer*>(monitor)->inforReceiveLineFromCompetitor(nLinesRemove);
    }
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
}