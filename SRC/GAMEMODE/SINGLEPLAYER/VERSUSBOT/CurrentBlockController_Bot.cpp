#include "CurrentBlockController_Bot.hpp"

#include "CurrentBlock_Bot.hpp"
#include "CurrentBlock_Multiplayer.hpp"

CurrentBlockController_Bot::CurrentBlockController_Bot(Map* map) : CurrentBlockController(map) {}

void CurrentBlockController_Bot::createCurrentBlock() {
    block = new CurrentBlock_Bot();
}

CurrentBlockController_Bot::~CurrentBlockController_Bot() {}