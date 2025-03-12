#ifndef COMMON_H
#define COMMON_H

#define BLOCK_EDGE 4

#define getBit(bit, i) (((bit) >> (i))&1)
#define MASK(x)        (1ll << (x))
#define FULLMASK(x)    (MASK(x) - 1)

#define getMask(mask, i) (((mask) >> (i)) & FULLMASK(BLOCK_EDGE))
#define getLine(shape, i) getMask(shape, (3 - i) * BLOCK_EDGE)

#define SIZEBLOCK 16

const int BLOCK_SIZE      = 20;
const int WIDTH_BORDER    = 5;

const int X_COORDINATE    = 50; 
const int Y_COORDINATE    = 50; 

const int HOLD_POSITION_X = X_COORDINATE;
const int HOLD_POSITION_Y = Y_COORDINATE + 5 * BLOCK_SIZE;
const int HOLD_WIDTH      = 5;
const int HOLD_HEIGHT     = 3;

const int GRID_POSITION_X = HOLD_POSITION_X + BLOCK_SIZE * (HOLD_WIDTH + 1);
const int GRID_POSITION_Y = Y_COORDINATE;
const int GRID_WIDTH      = 10;
const int GRID_HEIGHT     = 24;

const int NEXT_POSITION_X = GRID_POSITION_X + BLOCK_SIZE * (GRID_WIDTH + 1);
const int NEXT_POSITION_Y = Y_COORDINATE + 5 * BLOCK_SIZE;
const int NEXT_WIDTH      = 5;
const int NEXT_HEIGHT     = 15;


#endif