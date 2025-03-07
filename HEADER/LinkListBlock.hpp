#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
// #include <ncurses.h>
#include <SFML/Graphics.hpp>

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
    uint8_t nEle;
    
    void addEle();
    
    public:
    LinkListBlock();
    ~LinkListBlock();
    Block* updateNext();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif