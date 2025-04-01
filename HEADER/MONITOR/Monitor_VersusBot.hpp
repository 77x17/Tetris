#pragma once

#include "Monitor_Multiplayer.hpp"

class CurrentBlock;

class Monitor_VersusBot: public Monitor_Multiplayer {
private:
public:
    Monitor_VersusBot(int X_COORDINATE, int Y_COORDINATE); // Constructor
    ~Monitor_VersusBot(); // Destructor
    virtual void CreateMonitor(int X_COORDINATE, int Y_COORDINATE);
    void findPath(CurrentBlock*);
};