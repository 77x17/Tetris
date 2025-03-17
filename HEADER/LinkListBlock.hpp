#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <random>

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

    uint8_t   nEle;
    BlockEle *head;
    BlockEle *tail;
    sf::Font  font;

    std::mt19937 gen;
    void addBag();
    
public:
    LinkListBlock(int NEXT_POSITION_X, int NEXT_POSITION_Y, int NEXT_WIDTH, int NEXT_HEIGHT);
    ~LinkListBlock();
    void setSeed(int seed);
    void reset(uint32_t seed);

    void addNode(Block *block);
    Block* updateNext();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};

#endif