#ifndef PLAYERWITHBOT_HPP
#define PLAYERWITHBOT_HPP

#include <SFML/Window/Event.hpp>

#include "Player.hpp"

class Bot;

class PlayerWithBot: public Player{
private:
    
public:
    PlayerWithBot(int X_COORDINATE, int Y_COORDINATE);
    ~PlayerWithBot();
    
    void setCompetitor(Monitor*);
    
    void initialize();
    void setGameOver();
    void start(uint32_t seed, Bot* b);
};

#endif