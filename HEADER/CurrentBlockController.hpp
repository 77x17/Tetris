#ifndef CURRENTBLOCKCONTROLLER_HPP
#define CURRENTBLOCKCONTROLLER_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class CurrentBlock;
class Map;
class Block;
class Hold;
class SoundManager;

class CurrentBlockController {
private:
    CurrentBlock *block;
    
    SoundManager *soundManager;
    
    bool spin; // check piece spin

    void updateShadow(Map* map);
    bool isValid(uint16_t shape, Map* map);
public:
    CurrentBlockController();
    // CurrentBlockController(CurrentBlock *p);
    ~CurrentBlockController();

    bool isJustSpin();

    bool isEmpty();
    void setter(Block* p);

    void resetPosition(Map* map);

    bool moveDown(Map* map);
    bool fallDown(Map* map);
    bool moveLeft(Map* map);
    bool moveRight(Map* map);
    bool hardDrop(Map* map);

    bool collisionBottom(Map *map);

    bool rotateLeft(Map* map);
    bool rotateRight(Map* map);

    void swap(Hold* hold);

    uint8_t putIntoMap(Map* map);
    void draw(sf::RenderWindow *window, Map *map);

    char getTypeBlock();
    CurrentBlock* getCurrentBlock() const;

    bool gameOver(Map* map);

    void compress(sf::Packet &packet);
    void compressWithSpin(sf::Packet &packet);
};

#endif