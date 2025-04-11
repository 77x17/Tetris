#pragma once

#include "Monitor_Multiplayer.hpp"

class CurrentBlock_Bot;

class Monitor_VersusBot: public Monitor_Multiplayer {
private:
public:
    Monitor_VersusBot(int X_COORDINATE, int Y_COORDINATE); // Constructor
    ~Monitor_VersusBot(); // Destructor

    void updateScoreForBrainBot();
    virtual void createMonitor(int X_COORDINATE, int Y_COORDINATE);
    void findPath(int8_t &X, int8_t &timeRotate,  bool &isHold, CurrentBlock_Bot*);
};