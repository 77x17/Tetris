#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <SFML/Graphics.hpp>

class Next;
class LinkListBlock;
class Map;
class Hold;
class Block;
class CurrentBlock;
class SoundManager;

class Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;

    bool gameOver;
    
protected:
    Hold             *hold;
    LinkListBlock    *next;
    SoundManager     *soundManager;

public:
    Monitor(int X_COORDINATE, int Y_COORDINATE);
    virtual void createMonitor(int X_COORDINATE, int Y_COORDINATE) = 0;

    virtual ~Monitor();
    
    virtual void resetMonitor(uint32_t seed);
    virtual void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;

    virtual uint8_t removeNLines(int nLines, CurrentBlock* currentBlock, bool isAllClear) = 0;
    
    bool isGameOver();
    void setGameOver();

    bool canHold();
    void unlockHold();
    void lockHold();
    virtual void exchangeCurrentBlock(CurrentBlock*);

    virtual Map* getMap() const = 0;

    virtual int putIntoMap(CurrentBlock* curBlock) = 0;
    Block* getNext() const;

    virtual void setTimer() = 0;
    virtual void pauseTimer() = 0;
    virtual void unPauseTimer() = 0;
};

#endif