#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <ncurses.h>

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

class Monitor {
protected:
    Hold* hold;
    CurrentBlock* curBlock;
    Map* map;
    Infor* infor;
    LinkListBlock* next;
    WINDOW* win;
    
public:
    Monitor(WINDOW* win);
    ~Monitor();
    bool moveProcessing();
    virtual void start()=0;
};

#endif