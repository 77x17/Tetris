#include "Map.hpp"

#include "Block.hpp"
#include "Common.hpp"
#include "CommonMap.hpp"
#include "CurrentBlock.hpp"

#include <random>

Map::Map() {
    texture.loadFromFile("ASSETS/blocks/blocks.png");
    map = new uint64_t[Common::HEIGHT_MAP + 1]();
    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        map[i] = EMPTYLINE;
    }
    map[Common::HEIGHT_MAP] = FULLMASK(REALWIDTH);
}

void Map::setPosition(int x, int y, int w, int h) { GRID_POSITION_X = x; GRID_POSITION_Y = y; GRID_WIDTH = w; GRID_HEIGHT = h; }

Map::~Map() {
    delete[] map;
}

void Map::reset() {
    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        map[i] = EMPTYLINE;
    }
    map[Common::HEIGHT_MAP] = FULLMASK(REALWIDTH);
}

void Map::drawOutline(sf::RenderWindow* window) {
    sf::RectangleShape line;

    // Grid
    line.setSize(sf::Vector2f(GRID_WIDTH * Common::BLOCK_SIZE, 1));
    line.setFillColor(sf::Color(100, 100, 100, 100)); // Gray
    for (int i = 4; i <= GRID_HEIGHT; i++) {
        line.setPosition(GRID_POSITION_X, GRID_POSITION_Y + i * Common::BLOCK_SIZE);
        window->draw(line);
    }

    // Grid
    line.setSize(sf::Vector2f(1, (GRID_HEIGHT - 4) * Common::BLOCK_SIZE));
    line.setFillColor(sf::Color(100, 100, 100, 100)); // Gray
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line.setPosition(GRID_POSITION_X + i * Common::BLOCK_SIZE, 4 * Common::BLOCK_SIZE + GRID_POSITION_Y);
        window->draw(line);
    }

    // Lower line
    line.setSize(sf::Vector2f(GRID_WIDTH * Common::BLOCK_SIZE + Common::WIDTH_BORDER + Common::WIDTH_BORDER, 5));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - Common::WIDTH_BORDER, GRID_POSITION_Y + GRID_HEIGHT * Common::BLOCK_SIZE);
    window->draw(line);
    
    // Left - right line
    line.setSize(sf::Vector2f(Common::WIDTH_BORDER, (GRID_HEIGHT - 4) * Common::BLOCK_SIZE));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - Common::WIDTH_BORDER                          , 4 * Common::BLOCK_SIZE + GRID_POSITION_Y);
    window->draw(line);
    line.setPosition(GRID_POSITION_X                + GRID_WIDTH * Common::BLOCK_SIZE, 4 * Common::BLOCK_SIZE + GRID_POSITION_Y);
    window->draw(line);
}

void Map::remove(uint8_t pos) {
    for (int i = pos - 1; i >= 0; i--) 
        map[i + 1] = map[i];
    map[0] = EMPTYLINE;
}

void Map::draw(sf::RenderWindow *window) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(Common::BLOCK_SIZE - 1, Common::BLOCK_SIZE - 1));
    block.setTexture(&texture);
    for (int i = 0; i < Common::HEIGHT_MAP; i++) {
        for (int j = 0; j < Common::WIDTH_MAP; j++) if (getBit(map[i], j + NUMOFFSET)) {
            block.setPosition(GRID_POSITION_X + j * Common::BLOCK_SIZE + 1, GRID_POSITION_Y + i * Common::BLOCK_SIZE + 1);
            uint8_t shapeID = ((map[i] >> (j * COLORWIDTH + REALWIDTH)) & FULLMASK(4));
            block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));
            window->draw(block);
        }
    }
}

void Map::drawCurrentBlock(sf::RenderWindow* window, CurrentBlock *curBlock) {
    curBlock->draw(window, GRID_POSITION_Y, GRID_POSITION_X);
}

uint8_t Map::putBlockIntoMap(Block* block, int Y, int X) {
    uint8_t cnt = 0;
    uint16_t shape = block->getShape();
    uint64_t color = block->getShapeID();

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < Common::HEIGHT_MAP) {
        if (map[Y + i] & (getLine(shape, i) << (X + NUMOFFSET))) {
            i = -1;
            Y--;
        }
    }

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < Common::HEIGHT_MAP) {
        map[Y + i] ^= (getLine(shape, i) << (X + NUMOFFSET));
        if (((map[Y + i] & FULLMASK(REALWIDTH)) ^ FULLMASK(REALWIDTH)) == 0) {
            remove(Y + i);
            cnt++;
        }
        else {
            for (int j = 0; j < BLOCK_EDGE; j++)
                if (getBit(getLine(shape, i), j))
                    map[Y + i] ^= (color << ((X + j) * COLORWIDTH + REALWIDTH));
        }
    }

    return cnt;
}

bool Map::isValid(uint16_t shape, int Y, int X) {
    if (X < -NUMOFFSET || X + BLOCK_EDGE > Common::WIDTH_MAP + NUMOFFSET) return false;
    for (int i = 0; i < BLOCK_EDGE; i++)
        if (((getMask(map[i + Y], X + NUMOFFSET) ^ getLine(shape, i)) & getMask(map[i + Y], X + NUMOFFSET)) != getMask(map[i + Y], X + NUMOFFSET))
            return false;
    return true;
}

bool Map::isAllClear() { return map[Common::HEIGHT_MAP - 1] == EMPTYLINE; }