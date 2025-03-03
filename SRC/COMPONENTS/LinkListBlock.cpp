#include "LinkListBlock.hpp"

BlockEle::BlockEle() { p = nullptr; next = nullptr; }
BlockEle::BlockEle(Block* tmp) { p = tmp; next = nullptr; }

LinkListBlock::LinkListBlock(WINDOW* _win) {
    win = derwin(_win, 14, 6, 0, 20);
    head = tail = new BlockEle(BlockFactory::createRandomBlock());

    for (int i = 1; i <= 3; i++) {
        tail->next = new BlockEle(BlockFactory::createRandomBlock());
        tail = tail->next;
    }
}

LinkListBlock::~LinkListBlock() {
    if (head == nullptr) return;
    while (head) {
        BlockEle* tmp = head; head = head->next;
        delete tmp->p; // this is a block create in factory and it is not automatically deleted.
        delete tmp;
    }
    head = tail = nullptr;
}

void LinkListBlock::updateNext(CurrentBlock* cur) {
    cur = new CurrentBlock(head->p); 

    BlockEle* p = head; 
    head = head->next;
    delete p;
}