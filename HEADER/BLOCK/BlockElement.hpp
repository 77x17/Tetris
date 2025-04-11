#ifndef BLOCKELEMENT_HPP
#define BLOCKELEMENT_HPP

class Block;

class BlockElement{
private:
    Block* block;
    BlockElement* next;
public:
    BlockElement();
    BlockElement(Block* p);
    friend class LinkListBlock;
    friend class LinkListBlock_Bot;
    friend class BlockFactory;
};

#endif