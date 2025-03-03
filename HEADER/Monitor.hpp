#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <ncurses.h>

#include "Hold.hpp"
#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Infor.hpp"

class Monitor {
protected:
    CurrentBlock *curBlock;
    Hold* hold;
    LinkListBlock* next;
    Map* map;
    Infor* infor;
    WINDOW* win;
    
public:
    Monitor(WINDOW* win);
    ~Monitor();
    bool moveProcessing();
    virtual void start()=0;
};

#endif