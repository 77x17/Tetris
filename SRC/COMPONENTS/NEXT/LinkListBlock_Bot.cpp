#include "LinkListBlock_Bot.hpp"

#include "Block.hpp"
#include "BlockElement.hpp"

#include <iostream>

LinkListBlock_Bot::LinkListBlock_Bot(): LinkListBlock(){}
LinkListBlock_Bot::~LinkListBlock_Bot(){}

Block* LinkListBlock_Bot::getBlock() {
	if (nEle < 7) addBag();
	return new Block(*head->block);
}