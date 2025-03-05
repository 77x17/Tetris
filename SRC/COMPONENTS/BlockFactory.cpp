#include "BlockFactory.hpp"

#include "Block.hpp"

Block* BlockFactory::createRandomBlock() {
    // // Danh sách các loại block
    enum BlockType { Z_BLOCK, T_BLOCK, S_BLOCK, L_BLOCK, P_BLOCK, O_BLOCK, I_BLOCK};
    
    // Tạo số ngẫu nhiên
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 6);
    
    // Tạo block dựa trên kết quả ngẫu nhiên
    switch(distr(gen)) {
        case Z_BLOCK: return new ZBlock();
        case T_BLOCK: return new TBlock();
        case S_BLOCK: return new SBlock();
        case L_BLOCK: return new LBlock();
        case P_BLOCK: return new PBlock();
        case O_BLOCK: return new OBlock();
        case I_BLOCK: return new IBlock();
        default: nullptr;
    }
}