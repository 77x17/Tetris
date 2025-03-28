#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <SFML/Graphics.hpp>

class Infor;
class Next;
class LinkListBlock;
class Map;
class Hold;
class CurrentBlock;

class Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;

    bool gameOver;
    
protected:
    Hold             *hold;
    LinkListBlock    *next;

public:
    Monitor(int X_COORDINATE, int Y_COORDINATE);
    virtual void CreateMonitor(int X_COORDINATE, int Y_COORDINATE) = 0;

    virtual ~Monitor();
    
    virtual void resetMonitor(uint32_t seed);
    virtual void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;

    virtual uint8_t removeNLines(int nLines, CurrentBlock* currentBlock) = 0;
    
    bool isGameOver();
    void setGameOver();

    bool canHold();
    void unlockHold();
    void lockHold();
    virtual void exchangeCurrentBlock(CurrentBlock*);

    virtual Map* getMap() const = 0;

    virtual int putIntoMap(CurrentBlock* curBlock) = 0;
    LinkListBlock* getNext() const;
};

#endif