#include "LinkListBlock.hpp"
#include <iostream>

#include "Block.hpp"
#include "BlockFactory.hpp"
#include "CurrentBlock.hpp"

BlockEle::BlockEle() { block = nullptr; next = nullptr; }
BlockEle::BlockEle(Block* tmp) { block = tmp; next = nullptr; }

void LinkListBlock::addEle() {
    if (tail == nullptr) head = tail = new BlockEle(BlockFactory::createRandomBlock());
    else {
        tail->next = new BlockEle(BlockFactory::createRandomBlock());
        tail = tail->next;
    }
    nEle++;
}

LinkListBlock::LinkListBlock(WINDOW* _win) {
    win = derwin(_win, 14, 6, 2, 24);
    box(win, 0, 0);
    mvwaddstr(win, 0, 1, "NEXT");
    wrefresh(win);
    
    for (int i = 1; i <= 3; i++)
        addEle();
}

LinkListBlock::~LinkListBlock() {
    if (head == nullptr) return;
    while (head) {
        BlockEle* tmp = head; head = head->next;
        delete tmp->block; // this is a block create in factory and it is not automatically deleted.
        delete tmp;
    }
    head = tail = nullptr;
}

CurrentBlock* LinkListBlock::updateNext() {
    CurrentBlock* cur = new CurrentBlock(head->block); 

    BlockEle* p = head; 
    head = head->next;
    delete p; 
    if (--nEle < 3) addEle();
    draw();
    return cur;
}

void LinkListBlock::draw() {
    BlockEle *p = head;
    for (int i = 0; i < 3; i++) {
        p->block->draw(win, 1 + i * 4, 1, true);
        p = p->next;
    }
}