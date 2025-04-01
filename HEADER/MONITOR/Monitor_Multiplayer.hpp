#ifndef MONITORFORTWOPLAYER_HPP
#define MONITORFORTWOPLAYER_HPP

#include "Monitor.hpp"

class Hold;
class LinkListBlock;
class Map_Multiplayer;
class Infor_Multiplayer;
class CurrentBlock;

class Monitor_Multiplayer: public Monitor {
protected:
    Map_Multiplayer     *map;
    Infor_Multiplayer   *infor;
public:
    Monitor_Multiplayer(int X_COORDINATE, int Y_COORDINATE);
    virtual ~Monitor_Multiplayer();
    virtual void CreateMonitor(int X_COORDINATE, int Y_COORDINATE);
    
    void setPosition(int X_COORDINATE, int Y_COORDINATE);

    void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;
    void resetMonitor(uint32_t seed);

    uint8_t removeNLines(int nLines, CurrentBlock* currentBlock, bool isAllClear);

    void setNewSeed(int seed);
    void exchangeCurrentBlock(CurrentBlock*);

    void inforReceiveLineFromCompetitor(int nLines);
    void mapReceiveLineFromCompetitor(int seed);

    Map* getMap() const;
    int putIntoMap(CurrentBlock* curBlock);

    void setTimer();
    void pauseTimer();
    void unPauseTimer();
};

#endif