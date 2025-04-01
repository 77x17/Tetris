#include "Monitor_VersusBot.hpp"

#include "CurrentBlock.hpp"
#include "Map_Bot.hpp"
#include "Hold.hpp"
#include "Infor_Multiplayer.cpp"
#include "LinkListBlock.hpp"

Monitor_VersusBot::Monitor_VersusBot(int x, int y): Monitor_Multiplayer(x, y) {}

Monitor_VersusBot::~Monitor_VersusBot() {}

void Monitor_Multiplayer::CreateMonitor(int x, int y) {
    hold = new Hold();
    map  = new Map_Bot();
    next = new LinkListBlock();
    infor = new Infor_Multiplayer();
    setPosition(x, y);
}


void Monitor_VersusBot::findPath(CurrentBlock* curBlock) {
    // curBlock->
}