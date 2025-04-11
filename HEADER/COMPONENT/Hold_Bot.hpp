#ifndef HOLD_BOT_HPP
#define HOLD_BOT_HPP

#include "Hold.hpp"

class Block;

class Hold_Bot : public Hold {
public:
	Hold_Bot();
	~Hold_Bot();
	Block* getBlock();
};

#endif // HOLD_BOT_HPP