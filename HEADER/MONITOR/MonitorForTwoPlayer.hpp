#ifndef MONITORFORTWOPLAYER_HPP
#define MONITORFORTWOPLAYER_HPP

#include "Monitor.hpp"

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

class MonitorForTwoPlayer: public Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
public:
    MonitorForTwoPlayer(int X_COORDINATE, int Y_COORDINATE);
    void CreateMonitor(int X_COORDINATE, int Y_COORDINATE);
    void setPosition(int X_COORDINATE, int Y_COORDINATE);

    void setNewSeed(int seed);

    void inforReceiveLineFromCompetitor(int nLines);
    void mapReceiveLineFromCompetitor(int seed);
};

#endif