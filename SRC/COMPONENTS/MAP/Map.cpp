#include "Map.hpp"

#include "Block.hpp"

#define OFFSETX 4
#define OFFSETY 0
#define OFFRIGHT 12
#define NUMOFFSET 2
#define REALWIDTH 14
#define COLORWIDTH 4 // NUMBIT FOR COLOR -> pos[1] in ith line have color in bit 14th -> 17th

// **--------** REALWIDTH
// ** NUMOFFSET 
// **-------- OFFRIGHT

#define EMPTYLINE (FULLMASK(NUMOFFSET) ^ (FULLMASK(NUMOFFSET) << OFFRIGHT))

Map::Map(int x, int y, int w, int h) : GRID_POSITION_X(x), GRID_POSITION_Y(y), GRID_WIDTH(w), GRID_HEIGHT(h) {
    texture.loadFromFile("ASSETS/blocks/blocks.png");
    map = new uint64_t[HEIGHT_MAP + 1]();
    for (int i = 0; i < HEIGHT_MAP; i++) {
        map[i] = EMPTYLINE;
    }
    map[HEIGHT_MAP] = FULLMASK(REALWIDTH);
}

Map::~Map() {
    delete map;
}

void Map::reset() {
    for (int i = 0; i < HEIGHT_MAP; i++) {
        map[i] = EMPTYLINE;
    }
    map[HEIGHT_MAP] = FULLMASK(REALWIDTH);
}

void Map::drawOutline(sf::RenderWindow* window) {
    sf::RectangleShape line;

    // Grid
    line.setSize(sf::Vector2f(GRID_WIDTH * BLOCK_SIZE, 1));
    line.setFillColor(sf::Color(100, 100, 100, 100)); // Gray
    for (int i = 4; i <= GRID_HEIGHT; i++) {
        line.setPosition(GRID_POSITION_X, GRID_POSITION_Y + i * BLOCK_SIZE);
        window->draw(line);
    }

    // Grid
    line.setSize(sf::Vector2f(1, (GRID_HEIGHT - 4) * BLOCK_SIZE));
    line.setFillColor(sf::Color(100, 100, 100, 100)); // Gray
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line.setPosition(GRID_POSITION_X + i * BLOCK_SIZE, 4 * BLOCK_SIZE + GRID_POSITION_Y);
        window->draw(line);
    }

    // Lower line
    line.setSize(sf::Vector2f(GRID_WIDTH * BLOCK_SIZE + WIDTH_BORDER + WIDTH_BORDER, 5));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - WIDTH_BORDER, GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE);
    window->draw(line);
    
    // Left - right line
    line.setSize(sf::Vector2f(WIDTH_BORDER, (GRID_HEIGHT - 4) * BLOCK_SIZE));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - WIDTH_BORDER                          , 4 * BLOCK_SIZE + GRID_POSITION_Y);
    window->draw(line);
    line.setPosition(GRID_POSITION_X                + GRID_WIDTH * BLOCK_SIZE, 4 * BLOCK_SIZE + GRID_POSITION_Y);
    window->draw(line);

    // Draw for garbage

    // Lower line
    line.setSize(sf::Vector2f(BLOCK_SIZE, 5));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - WIDTH_BORDER - BLOCK_SIZE, GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE);
    window->draw(line);
    
    // Left line
    line.setSize(sf::Vector2f(WIDTH_BORDER, (GRID_HEIGHT - 4) * BLOCK_SIZE));
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setPosition(GRID_POSITION_X - WIDTH_BORDER - BLOCK_SIZE , 4 * BLOCK_SIZE + GRID_POSITION_Y);
    window->draw(line);
}

void Map::remove(uint8_t pos) {
    for (int i = pos - 1; i >= 0; i--) 
        map[i + 1] = map[i];
    map[0] = EMPTYLINE;
}

bool Map::addPosible(uint8_t nLines) {
    for (int i = 0; i < nLines; i++) 
        if (map[i] != EMPTYLINE)
            return false;
    return true;
}
#include <iostream>
void Map::add(int nLines) {
    std::cerr << (int)nLines << '\n';
    if (nLines == 0) return;
    for (int i = 0; i + nLines < HEIGHT_MAP; i++)
        map[i] = map[i + nLines];
    for (int i = HEIGHT_MAP - nLines; i < HEIGHT_MAP; i++) 
        map[i] = FULLMASK(REALWIDTH);
}


void Map::draw(sf::RenderWindow *window) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    block.setTexture(&texture);
    for (int i = 0; i < HEIGHT_MAP; i++) {
        for (int j = 0; j < WIDTH_MAP; j++) if (getBit(map[i], j + NUMOFFSET)) {
            block.setPosition(GRID_POSITION_X + j * BLOCK_SIZE + 1, GRID_POSITION_Y + i * BLOCK_SIZE + 1);
            uint8_t shapeID = ((map[i] >> (j * COLORWIDTH + REALWIDTH)) & FULLMASK(4));
            block.setTextureRect(sf::IntRect(shapeID * 25, 0, 25, 25));
            window->draw(block);
        }
    }
}

void Map::drawCurrentBlock(sf::RenderWindow* window, Block *block, int posY, int shadowPosY, int posX) {
    block->drawGhost(window, shadowPosY, posX, GRID_POSITION_Y, GRID_POSITION_X);
    block->draw(window, posY, posX, GRID_POSITION_Y, GRID_POSITION_X);
}

uint8_t Map::update(Block* block, int Y, int X) {
    uint8_t cnt = 0;
    uint16_t shape = block->getShape();
    uint64_t color = block->getShapeID();

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < HEIGHT_MAP) {
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
    if (X < -NUMOFFSET || X + BLOCK_EDGE > HEIGHT_MAP + NUMOFFSET) return false;

    for (int i = 0; i < BLOCK_EDGE; i++)
        if (((getMask(map[i + Y], X + NUMOFFSET) ^ getLine(shape, i)) & getMask(map[i + Y], X + NUMOFFSET)) != getMask(map[i + Y], X + NUMOFFSET))
            return false;
    return true;
}