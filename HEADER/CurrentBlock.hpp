#ifndef CURRENTBLOCK_HPP
#define CURRENTBLOCK_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>

class Block;
class Map;
class Hold;


class CurrentBlock {
private:
    Block* block;
    int8_t posX, posY;
    int8_t shadowPosY;

    void shadowHardDrop(Map *map);
public:
    CurrentBlock();
    CurrentBlock(Block *p);
    ~CurrentBlock();

    bool isEmpty();
    void setter(Block* p);
    bool resetPosition(Map *map);

    bool moveDown(Map *map);
    bool moveLeft(Map *map);
    bool moveRight(Map *map);
    bool hardDrop(Map *map);

    bool collisionBottom(Map *map);

    bool rotateLeft(Map* map);
    bool rotateRight(Map* map);

    void swap(Hold* hold);

    uint8_t put(Map* map);
    void draw(sf::RenderWindow *window, Map *map);
    bool isValid(uint16_t shape, Map* map);

    bool gameOver(Map* map);
};

#endif