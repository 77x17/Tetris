#ifndef COMMON_H
#define COMMON_H


#define HEIGHT 24
#define WIDTH 10


#define BLOCK_EDGE 4

#define getBit(bit, i) (((bit) >> (i))&1)
#define MASK(x) (1ll << (x))
#define FULLMASK(x) (MASK(x) - 1)

#define getMask(mask, i) (((mask) >> (i)) & FULLMASK(BLOCK_EDGE))
#define getLine(shape, i) getMask(shape, (3 - i) * BLOCK_EDGE)

#define SIZEBLOCK 16


#endif