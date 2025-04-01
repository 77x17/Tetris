#ifndef CURRENTBLOCKCONTROLLER_BOT_HPP
#define CURRENTBLOCKCONTROLLER_BOT_HPP

#include "CurrentBlockController.hpp"

class CurrentBlockController_Bot: public CurrentBlockController {
public:
    CurrentBlockController_Bot(Map* map);
    virtual ~CurrentBlockController_Bot();

    virtual void createCurrentBlock();
};


#endif