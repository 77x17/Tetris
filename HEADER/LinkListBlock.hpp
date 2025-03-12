#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <SFML/Graphics.hpp>

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
    friend class BlockFactory;
};

class LinkListBlock {
private:
    int NEXT_POSITION_X; 
    int NEXT_POSITION_Y;
    int NEXT_WIDTH;
    int NEXT_HEIGHT;

    BlockEle* head;
    BlockEle* tail;
    uint8_t nEle;
    
    void addBag();
    
public:
    LinkListBlock(int NEXT_POSITION_X, int NEXT_POSITION_Y, int NEXT_WIDTH, int NEXT_HEIGHT);
    ~LinkListBlock();
    void addNode(Block *block);
    Block* updateNext();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif