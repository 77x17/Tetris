#ifndef SETPIECE_H
#define SETPIECE_H

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <random>
#include "Monitor.hpp"

class Block;
class BlockEle;

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

    void setPosition(int NEXT_POSITION_X, int NEXT_POSITION_Y, int NEXT_WIDTH, int NEXT_HEIGHT);
    
public:
    LinkListBlock();
    ~LinkListBlock();
    void setSeed(int seed);
    void reset(uint32_t seed);

    void addNode(Block *block);
    Block* updateNext();
    void drawOutline(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);

    friend void Monitor::setPosition(int X_COORDINATE, int Y_COORDINATE);
};

#endif