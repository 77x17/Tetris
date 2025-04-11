#include "Monitor_VersusBot.hpp"

#include "CurrentBlock_Bot.hpp"
#include "Map_Bot.hpp"
#include "Hold_Bot.hpp"
#include "Infor_VersusBot.hpp"
#include "LinkListBlock_Bot.hpp"

#include "BotBrain.hpp"

Monitor_VersusBot::Monitor_VersusBot(int x, int y): Monitor_Multiplayer(x, y) {}

Monitor_VersusBot::~Monitor_VersusBot() {}

void Monitor_VersusBot::createMonitor(int x, int y) {
    hold = new Hold_Bot();
    map  = new Map_Bot();
    next = new LinkListBlock_Bot();
    infor = new Infor_VersusBot();
    setPosition(x, y);
}

void Monitor_VersusBot::updateScoreForBrainBot() {
    // infor->
}

void Monitor_VersusBot::findPath(int8_t &X, int8_t &timeRotate, bool &isHold, CurrentBlock_Bot* curBlock) {
    isHold = false;
    CurrentBlock_Bot* holdTmp = new CurrentBlock_Bot(dynamic_cast<Hold_Bot*>(hold)->getBlock());
    if (holdTmp->isEmpty()) holdTmp->freeAndSetter(dynamic_cast<LinkListBlock_Bot*>(next)->getBlock());
    dynamic_cast<Map_Bot*>(map)->findPath(X, timeRotate, isHold, curBlock, holdTmp);
    delete holdTmp;
}