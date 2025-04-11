#include "Hold_Bot.hpp"

#include "Block.hpp"

Hold_Bot::Hold_Bot():Hold(){}
Hold_Bot::~Hold_Bot(){}

Block* Hold_Bot::getBlock() {
	if (block == nullptr) return nullptr;
	return new Block(*block);
}