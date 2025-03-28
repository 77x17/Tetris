#ifndef MONITORFORONEPLAYER_HPP
#define MONITORFORONEPLAYER_HPP

#include "Monitor.hpp"

class Map;
class Infor;
class CurrentBlock;

class MonitorForOnePlayer: public Monitor {
private:
    Map          *map;
    Infor        *infor;
public:
    MonitorForOnePlayer(int X_COORDINATE, int Y_COORDINATE);
    void CreateMonitor(int X_COORDINATE, int Y_COORDINATE);
    void setPosition(int X_COORDINATE, int Y_COORDINATE);

    void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;
    void resetMonitor(uint32_t seed);

    uint8_t removeNLines(int nLines, CurrentBlock* currentBlock);

    void exchangeCurrentBlock(CurrentBlock*);

    Map* getMap() const;
    int putIntoMap(CurrentBlock* curBlock);

    void setTimer();
    void pauseTimer();
    void unPauseTimer();
};

#endif