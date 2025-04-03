#include "Infor_Survival.hpp"

#include "Infor.hpp"

Infor_Survival::Infor_Survival():Infor_Multiplayer(){}
Infor_Survival::~Infor_Survival(){}

void Infor_Survival::update(uint8_t lines, bool spin, char typeBlock, bool isAllClear) {
    Infor::update(lines, spin, typeBlock, isAllClear);
    // check whether have player finishesd challenge!
    // ... continue write here
    
}

void Infor_Survival::draw(sf::RenderWindow *window) {
    Infor_Multiplayer::draw(window);
    // add draw question or something here!
    // ... addLines(...);
}