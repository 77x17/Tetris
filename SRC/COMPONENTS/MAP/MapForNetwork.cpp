#include "MapForNetwork.hpp"

#include "CommonMap.hpp"
#include "Block.hpp"
#include <random>

MapForNetwork::MapForNetwork():Map(){}
MapForNetwork::~MapForNetwork(){}

bool MapForNetwork::addPosible(uint64_t nLines) {
    // for (int i = 0; i < nLines; i++) 
    //     if (map[i] != EMPTYLINE)
    //         return false;
    return true;
}

void MapForNetwork::add(uint64_t nLinesAdd, int seed) {
    if (nLinesAdd == 0) return;

    if (getBit(nLinesAdd, 0) == 0) throw std::runtime_error("nLinesAdd have some problems");
    
    int nLines = __builtin_popcount(nLinesAdd);

    for (int i = 1; i + nLines < Common::HEIGHT_MAP; i++)
        map[i] = map[i + nLines];
    int p = Common::HEIGHT_MAP;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, Common::WIDTH_MAP - 1);
    int posException = dis(gen);

    while(nLinesAdd && p >= 1) {
        if (getBit(nLinesAdd, 0))
            map[--p] = (FULLMASK(REALWIDTH) ^ MASK(posException + NUMOFFSET));
        else {
            std::uniform_int_distribution<> dis(0, Common::WIDTH_MAP - 1);
            posException = dis(gen);
        }
        nLinesAdd >>= 1;
    }
}

uint8_t MapForNetwork::putBlockIntoMap(Block* block, int Y, int X) {
    uint16_t shape = block->getShape();

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < Common::HEIGHT_MAP) {
        if (map[Y + i] & (getLine(shape, i) << (X + NUMOFFSET))) {
            i = -1;
            Y--;
        }
    }

    return Map::putBlockIntoMap(block, Y, X);
}