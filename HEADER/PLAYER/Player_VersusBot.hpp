#pragma once

#include <SFML/Window/Event.hpp>

#include "Player.hpp"

class Bot;

class Player_VersusBot: public Player{
private:
    
public:
    Player_VersusBot(int X_COORDINATE, int Y_COORDINATE);
    ~Player_VersusBot();
    
    void setCompetitor(Monitor*);
    
    void initialize();
    void setGameOver();
    void start(uint32_t seed, Bot* b);
};
