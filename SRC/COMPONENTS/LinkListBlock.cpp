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

LinkListBlock::LinkListBlock(sf::RenderWindow* newWindow) {
    window = newWindow; //derwin(_win, 14, 6, 2, 24);
    // box(win, 0, 0);
    // mvwaddstr(win, 0, 1, "NEXT");
    // wrefresh(win);
    
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

Block* LinkListBlock::updateNext() {
    clean();

    Block* cur = head->block; 

    BlockEle* p = head; 
    head = head->next;
    delete p; 
    if (--nEle < 3) addEle();
    draw();
    return cur;
}

void LinkListBlock::drawOutline(sf::RenderWindow *window) {
    sf::RectangleShape line(sf::Vector2f(NEXT_WIDTH * BLOCK_SIZE, 1));
    line.setFillColor(sf::Color(200, 200, 200, 150)); // Gray
    line.setPosition(NEXT_POSITION_X, NEXT_POSITION_Y + 0           * BLOCK_SIZE);
    window->draw(line);
    line.setPosition(NEXT_POSITION_X, NEXT_POSITION_Y + NEXT_HEIGHT * BLOCK_SIZE);
    window->draw(line);
    
    line.setSize(sf::Vector2f(1, NEXT_HEIGHT * BLOCK_SIZE));
    line.setPosition(NEXT_POSITION_X + 0          * BLOCK_SIZE, NEXT_POSITION_Y);
    window->draw(line);
    line.setPosition(NEXT_POSITION_X + NEXT_WIDTH * BLOCK_SIZE, NEXT_POSITION_Y);
    window->draw(line);
}

void LinkListBlock::draw() {
    BlockEle *p = head;
    for (int i = 0; i < 3; i++) {
        p->block->draw(window, 1 + i * 4, 1);
        p = p->next;
    }
}

void LinkListBlock::clean() {
    BlockEle *p = head;
    for (int i = 0; i < 3; i++) {
        p->block->draw(window, 1 + i * 4, 1);
        p = p->next;
    }
}