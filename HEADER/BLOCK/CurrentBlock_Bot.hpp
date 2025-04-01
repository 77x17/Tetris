#ifndef CURRENTBLOCK_BOT_HPP
#define CURRENTBLOCK_BOT_HPP

#include <mutex>

#include "CurrentBlock.hpp"

class CurrentBlock_Bot: public CurrentBlock {
private:
    std::mutex mtx;
public:
    CurrentBlock_Bot();
    ~CurrentBlock_Bot();

};

#endif // CURRENTBLOCK_BOT_HPP