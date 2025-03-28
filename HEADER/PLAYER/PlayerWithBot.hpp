#ifndef PLAYERWITHBOT_HPP
#define PLAYERWITHBOT_HPP

#include <SFML/Window/Event.hpp>

#include "Player.hpp"

class PlayerWithBot: public Player{
private:
    
public:
    PlayerWithBot(int X_COORDINATE, int Y_COORDINATE, uint32_t seed);
    ~PlayerWithBot();

    void initialize();
    void setGameOver();
    void restart(uint32_t seed);
    void handleAddLine(uint8_t nLines);
};

#endif