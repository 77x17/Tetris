#pragma once

#include "Monitor.hpp"

class Map;
class Infor;
class CurrentBlock;

class Monitor_Singleplayer: public Monitor {
private:
    Map          *map;
    Infor        *infor;
public:
    Monitor_Singleplayer(int X_COORDINATE, int Y_COORDINATE);
    void createMonitor(int X_COORDINATE, int Y_COORDINATE);
    void setPosition(int X_COORDINATE, int Y_COORDINATE);

    void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;
    void resetMonitor(uint32_t seed);

    uint8_t removeNLines(int nLines, CurrentBlock* currentBlock, bool isAllClear);

    void exchangeCurrentBlock(CurrentBlock*);

    Map* getMap() const;
    int putIntoMap(CurrentBlock* curBlock);

    void setTimer();
    void pauseTimer();
    void unPauseTimer();
};