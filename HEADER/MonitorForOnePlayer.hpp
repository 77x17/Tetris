#ifndef MONITORFORONEPLAYER_HPP
#define MONITORFORONEPLAYER_HPP

#include "Monitor.hpp"

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

class MonitorForOnePlayer: public Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
public:
    MonitorForOnePlayer(int X_COORDINATE, int Y_COORDINATE);
    void CreateMonitor(int X_COORDINATE, int Y_COORDINATE);
    void setPosition(int X_COORDINATE, int Y_COORDINATE);
};

#endif