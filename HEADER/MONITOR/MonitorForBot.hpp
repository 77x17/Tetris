#ifndef MONITOR_FOR_BOT_HPP
#define MONITOR_FOR_BOT_HPP

#include "MonitorForTwoPlayer.hpp"

class CurrentBlock;

class MonitorForBot: public MonitorForTwoPlayer {
private:
public:
    MonitorForBot(int X_COORDINATE, int Y_COORDINATE); // Constructor
    ~MonitorForBot(); // Destructor
    void findPath(CurrentBlock*);
};

#endif // MONITOR_FOR_BOT_HPP