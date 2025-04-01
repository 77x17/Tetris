#include "Map_Multiplayer.hpp"

#include "CommonMap.hpp"
#include "Block.hpp"
#include <random>

Map_Multiplayer::Map_Multiplayer():Map(){}
Map_Multiplayer::~Map_Multiplayer(){}

bool Map_Multiplayer::addPosible(uint64_t nLines) {
    // for (int i = 0; i < nLines; i++) 
    //     if (map[i] != EMPTYLINE)
    //         return false;
    return true;
}

void Map_Multiplayer::add(uint64_t nLinesAdd, int seed) {
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

void Map_Multiplayer::drawOutline(sf::RenderWindow* window) {
    Map::drawOutline(window);
    sf::RectangleShape line;
    // Draw for garbage
    // Lower line
    line.setSize(sf::Vector2f(Common::BLOCK_SIZE, 5));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - Common::WIDTH_BORDER - Common::BLOCK_SIZE, GRID_POSITION_Y + GRID_HEIGHT * Common::BLOCK_SIZE);
    window->draw(line);
    
    // Left line
    line.setSize(sf::Vector2f(Common::WIDTH_BORDER, (GRID_HEIGHT - 4) * Common::BLOCK_SIZE));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - Common::WIDTH_BORDER - Common::BLOCK_SIZE , 4 * Common::BLOCK_SIZE + GRID_POSITION_Y);
    window->draw(line);
}


uint8_t Map_Multiplayer::putBlockIntoMap(Block* block, int Y, int X) {
    uint16_t shape = block->getShape();

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < Common::HEIGHT_MAP) {
        if (map[Y + i] & (getLine(shape, i) << (X + NUMOFFSET))) {
            i = -1;
            Y--;
        }
    }

    return Map::putBlockIntoMap(block, Y, X);
}