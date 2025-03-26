#include "CurrentBlockWithNetwork.hpp"

#include "CurrentBlock.hpp"
#include "Block.hpp"

CurrentBlockWithNetwork::CurrentBlockWithNetwork():CurrentBlock(){}
CurrentBlockWithNetwork::CurrentBlockWithNetwork(Block *p):CurrentBlock(p){}

CurrentBlockWithNetwork::~CurrentBlockWithNetwork() {}

