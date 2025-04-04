#pragma once

#include "Player.hpp"

class Player_Survival : public Player {
public:
    Player_Survival(int X_COORDINATE, int Y_COORDINATE);

    void initialize();

    void autoDown();

    void receiveGarbage(int x);

    void putGarbage();
};