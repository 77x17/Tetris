#include "BlockEle.hpp"

BlockEle::BlockEle() : block(nullptr), next(nullptr) {}
BlockEle::BlockEle(Block* tmp) : block(tmp), next(nullptr) {}