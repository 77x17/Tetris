#include "PlayerSurvival.hpp"

#include "MonitorForTwoPlayer.hpp"
#include "CurrentBlockController.hpp"
#include "MovementController.hpp"
#include "CurrentBlock.hpp"

#include <random>

PlayerSurvival::PlayerSurvival(int X_COORDINATE, int Y_COORDINATE) : Player(X_COORDINATE, Y_COORDINATE) {}

void PlayerSurvival::initialize() {
    monitor = new MonitorForTwoPlayer(X_COORDINATE, Y_COORDINATE); 
    monitor->CreateMonitor(X_COORDINATE, Y_COORDINATE);

    curBlock = new CurrentBlockController(dynamic_cast<MonitorForTwoPlayer*>(monitor)->getMap());
    movementController = new MovementController(monitor, curBlock);
}

void PlayerSurvival::receiveGarbage(int x) {
    dynamic_cast<MonitorForTwoPlayer *>(monitor)->inforReceiveLineFromCompetitor(4);
}

void PlayerSurvival::putGarbage() {
    std::random_device rd;
    std::mt19937 gen(rd());
    dynamic_cast<MonitorForTwoPlayer *>(monitor)->mapReceiveLineFromCompetitor(gen());
    curBlock->getCurrentBlock()->updateShadow(monitor->getMap());
}