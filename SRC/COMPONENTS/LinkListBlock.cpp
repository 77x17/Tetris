#include "LinkListBlock.hpp"

#include "Block.hpp"
#include "BlockFactory.hpp"
#include "CurrentBlock.hpp"

BlockEle::BlockEle() { block = nullptr; next = nullptr; }
BlockEle::BlockEle(Block* tmp) { block = tmp; next = nullptr; }

void LinkListBlock::addBag() {
    BlockFactory::createBag(head, tail);

    nEle += 7;
}

void LinkListBlock::addNode(Block *block) {
    if (tail == nullptr) head = tail = new BlockEle(block);
    else {
        tail->next = new BlockEle(block);
        tail = tail->next;
    }
    nEle++;
}

LinkListBlock::LinkListBlock() {
    nEle = 0; // bag size
    head = tail = nullptr;
    addBag();
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
    Block* cur = head->block; 

    BlockEle* p = head; 
    head = head->next;
    delete p; 
    if (--nEle < 7) addBag();
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

void LinkListBlock::draw(sf::RenderWindow *window) {
    BlockEle *p = head;
    for (int i = 0; i < 5; i++) {
        p->block->draw(window, i * 4, 0, NEXT_POSITION_Y, NEXT_POSITION_X, true);
        p = p->next;
    }
}
