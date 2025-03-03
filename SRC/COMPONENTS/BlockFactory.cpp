#include "BlockFactory.hpp"

Block* BlockFactory::createRandomBlock() {
    return new IBlock();
    // // Danh sách các loại block
    // enum BlockType { Z_BLOCK, T_BLOCK, S_BLOCK, TOTAL_TYPES };
    
    // // Tạo số ngẫu nhiên
    // static std::random_device rd;
    // static std::mt19937 gen(rd());
    // std::uniform_int_distribution<> distr(0, TOTAL_TYPES - 1);
    
    // // Tạo block dựa trên kết quả ngẫu nhiên
    // switch(distr(gen)) {
    //     case Z_BLOCK: return new Z_BLOCK();
    //     case T_BLOCK: return new T_BLOCK();
    //     case S_BLOCK: return new S_BLOCK();
    //     default: throw std::runtime_error("Unknown block type");
    // }
}