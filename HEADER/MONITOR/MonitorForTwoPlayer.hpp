#ifndef MONITORFORTWOPLAYER_HPP
#define MONITORFORTWOPLAYER_HPP

#include "Monitor.hpp"

class Hold;
class LinkListBlock;
class MapForNetwork;
class InforForNetwork;
class CurrentBlock;

class MonitorForTwoPlayer: public Monitor {
private:
    MapForNetwork     *map;
    InforForNetwork   *infor;
public:
    MonitorForTwoPlayer(int X_COORDINATE, int Y_COORDINATE);
    void CreateMonitor(int X_COORDINATE, int Y_COORDINATE);
    void setPosition(int X_COORDINATE, int Y_COORDINATE);

    void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;
    void resetMonitor(uint32_t seed);

    uint8_t removeNLines(int nLines, CurrentBlock* currentBlock);

    void setNewSeed(int seed);
    void exchangeCurrentBlock(CurrentBlock*);

    void inforReceiveLineFromCompetitor(int nLines);
    void mapReceiveLineFromCompetitor(int seed);

    Map* getMap() const;
    int putIntoMap(CurrentBlock* curBlock);
};

#endif