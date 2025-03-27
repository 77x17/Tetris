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
    SoundManager *soundManager;

    bool isValid(uint16_t shape);

    bool rotate(int8_t times);

protected:
    CurrentBlock *block;
    Map* map; // share pointer don't delete!

public:
    CurrentBlockController(Map* map);
    virtual ~CurrentBlockController();

    bool isEmpty();
    void setter(Block* p);

    bool isJustSpin();

    virtual bool moveDown();
    virtual bool fallDown();
    virtual bool moveLeft();
    virtual bool moveRight();
    virtual bool hardDrop();

    bool collisionBottom();

    virtual bool rotateLeft();
    virtual bool rotateRight();
    virtual bool rotate180();

    virtual void swap(Hold* hold);

    uint8_t putIntoMap();
    void draw(sf::RenderWindow *window);
    
    CurrentBlock* getCurrentBlock() const;

    bool gameOver();

    void compress(sf::Packet &packet);
    void compressWithSpin(sf::Packet &packet);
};

#endif