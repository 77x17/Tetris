#ifndef BLOCKELE_HPP
#define BLOCKELE_HPP

class Block;

class BlockEle{
private:
    Block* block;
    BlockEle* next;
public:
    BlockEle();
    BlockEle(Block* p);
    friend class LinkListBlock;
    friend class BlockFactory;
};

#endif