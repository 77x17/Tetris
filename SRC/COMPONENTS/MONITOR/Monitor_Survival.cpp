#include "Monitor_Survival.hpp"

#include "CurrentBlock.hpp"
#include "Map_Bot.hpp"
#include "Hold.hpp"
#include "Infor_Survival.hpp"
#include "LinkListBlock.hpp"

Monitor_Survival::Monitor_Survival(int x, int y): Monitor_Multiplayer() {}

Monitor_Survival::~Monitor_Survival() {}

void Monitor_Survival::createMonitor(int x, int y) {
    hold = new Hold();
    map  = new Map_Multiplayer();
    next = new LinkListBlock();
    infor = new Infor_Survival();
    setPosition(x, y);
}