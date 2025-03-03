#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <ncurses.h>

#include "Common.hpp"
#include "Block.hpp"
#include "BlockFactory.hpp"
#include "CurrentBlock.hpp"

class BlockEle{
private:
    Block* p;
    BlockEle* next;
public:
    BlockEle();
    BlockEle(Block* p);
    BlockEle* operator=(const Block* &BlockEle);
    friend class LinkListBlock;
};

class LinkListBlock {
private:
    BlockEle* head;
    BlockEle* tail;
    WINDOW* win;

public:
    LinkListBlock(WINDOW* win);
    ~LinkListBlock();
    void updateNext(CurrentBlock* cur);
};

#endif