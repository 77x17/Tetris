#include "Monitor_VersusBot.hpp"

#include "CurrentBlock_Bot.hpp"
#include "Map_Bot.hpp"
#include "Hold.hpp"
#include "Infor_Multiplayer.hpp"
#include "LinkListBlock.hpp"

Monitor_VersusBot::Monitor_VersusBot(int x, int y): Monitor_Multiplayer(x, y) {}

Monitor_VersusBot::~Monitor_VersusBot() {}

void Monitor_VersusBot::createMonitor(int x, int y) {
    hold = new Hold();
    map  = new Map_Bot();
    next = new LinkListBlock();
    infor = new Infor_Multiplayer();
    setPosition(x, y);
}


void Monitor_VersusBot::findPath(int8_t &X, CurrentBlock_Bot* curBlock) {
    dynamic_cast<Map_Bot*>(map)->findPath(X, curBlock);
}