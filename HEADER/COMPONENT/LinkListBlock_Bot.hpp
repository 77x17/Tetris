#ifndef LINKLISTBLOCK_BOT_HPP
#define LINKLISTBLOCK_BOT_HPP

#include "LinkListBlock.hpp"

class Block;

class LinkListBlock_Bot: public LinkListBlock {
public:
	LinkListBlock_Bot();
	~LinkListBlock_Bot();
	Block* getBlock();
};

#endif // LINKLISTBLOCK_BOT_HPP