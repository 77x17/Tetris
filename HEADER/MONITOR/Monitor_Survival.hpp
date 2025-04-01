#ifndef MONITOR_SURVIVAL_HPP
#define MONITOR_SURVIVAL_HPP

#include "Monitor_Multiplayer.hpp"

class Monitor_Survival: public Monitor_Multiplayer {
private:
public:
    Monitor_Survival(int X_COORDINATE, int Y_COORDINATE); // Constructor
    ~Monitor_Survival(); // Destructor

    void createMonitor(int X_COORDINATE, int Y_COORDINATE);
};

#endif // MONITOR_SURVIVAL_HPP