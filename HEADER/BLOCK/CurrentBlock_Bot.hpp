#ifndef CURRENTBLOCK_BOT_HPP
#define CURRENTBLOCK_BOT_HPP


#include <mutex>
#include "CurrentBlock.hpp"

class Block;

class CurrentBlock_Bot: public CurrentBlock {
private:
    std::mutex mtx;
public:
    CurrentBlock_Bot();
    CurrentBlock_Bot(Block* p);
    ~CurrentBlock_Bot();

    uint16_t getShape(int timesRotate);
    void rotate(int timesRotate);

};

#endif 