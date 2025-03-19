#include "BlockFactory.hpp"

#include "Block.hpp"
#include "LinkListBlock.hpp"

// Danh sách các loại block
enum BlockType { Z_BLOCK, T_BLOCK, S_BLOCK, L_BLOCK, J_BLOCK, O_BLOCK, I_BLOCK};

Block* BlockFactory::createRandomBlock() {
    // Tạo số ngẫu nhiên
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 6);
    
    // Tạo block dựa trên kết quả ngẫu nhiên
    return getBlockType(distr(gen));
}

Block* BlockFactory::getBlockType(int x) {
    switch(x) {
        case Z_BLOCK: return new ZBlock();
        case T_BLOCK: return new TBlock();
        case S_BLOCK: return new SBlock();
        case L_BLOCK: return new LBlock();
        case J_BLOCK: return new JBlock();
        case O_BLOCK: return new OBlock();
        case I_BLOCK: return new IBlock();
        default: return nullptr;
    }
}

void BlockFactory::createBag(BlockEle* &head, BlockEle* &tail, std::mt19937 &gen) {
    int number[7] = {0, 1, 2, 3, 4, 5, 6};
    std::shuffle(number, number + 7, gen);
    for (const int& x : number) { 
        if (head == nullptr) {
            head = tail = new BlockEle(getBlockType(x));
        }
        else {
            tail->next = new BlockEle(getBlockType(x));
            tail = tail->next;
        }
    }
}