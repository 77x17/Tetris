#pragma once

#include "Player.hpp"

class PlayerSurvival : public Player {
public:
    PlayerSurvival(int X_COORDINATE, int Y_COORDINATE);

    void initialize();

    void receiveGarbage(int x);

    void putGarbage();
};