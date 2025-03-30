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
    
    void setCompetitor(Bot*);
    
    void initialize();
    void setGameOver();
    void start(uint32_t seed);
    void handleAddLine(uint8_t nLines);
};

#endif