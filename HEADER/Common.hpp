#ifndef COMMON_H
#define COMMON_H

#define BLOCK_EDGE 4

#define getBit(bit, i) (((bit) >> (i))&1)
#define MASK(x)        (1ll << (x))
#define FULLMASK(x)    (MASK(x) - 1)

#define getMask(mask, i) (((mask) >> (i)) & FULLMASK(BLOCK_EDGE))
#define getLine(shape, i) getMask(shape, (3 - i) * BLOCK_EDGE)

#define SIZEBLOCK 16

const int BLOCK_SIZE   = 20;
const int WIDTH_BORDER = 5;

const int WIDTH_MAP  = 10;
const int HEIGHT_MAP = 24;

#endif