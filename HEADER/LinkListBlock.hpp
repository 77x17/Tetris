#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <ncurses.h>

#include "Common.hpp"

class BlockEle;
class Block;
class CurrentBlock;

class BlockEle{
private:
    Block* block;
    BlockEle* next;
public:
    BlockEle();
    BlockEle(Block* p);
    friend class LinkListBlock;
};

class LinkListBlock {
private:
    BlockEle* head;
    BlockEle* tail;
    WINDOW* win;
    uint8_t nEle;
    
    void addEle();
    
    public:
    LinkListBlock(WINDOW* win);
    ~LinkListBlock();
    Block* updateNext();
    void draw();
    void clean();
};

#endif