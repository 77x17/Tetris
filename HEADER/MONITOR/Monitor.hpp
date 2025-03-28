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
    bool gameOver;

protected:
    Hold             *hold;
    Map              *map;
    Infor            *infor;
    LinkListBlock    *next;

public:
    Monitor();
    virtual void CreateMonitor(int X_COORDINATE, int Y_COORDINATE) = 0;

    virtual ~Monitor();
    
    void resetMonitor(uint32_t seed);
    void draw(sf::RenderWindow* window, CurrentBlock* currentBlock) const;
    
    bool isGameOver();
    void setGameOver();

    uint8_t removeNLines(int nLines, CurrentBlock* currentBlock);

    bool canHold();
    void unlockHold();
    void lockHold();
    virtual void exchangeCurrentBlock(CurrentBlock*);

    Map* getMap() const;

    int putIntoMap(CurrentBlock* curBlock);
    LinkListBlock* getNext() const;
};

#endif