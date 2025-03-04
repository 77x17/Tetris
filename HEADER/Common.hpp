#ifndef COMMON_H
#define COMMON_H


#define HEIGHT 20
#define WIDTH 10
#define WIDTHREAL 16

#define BLOCK_EDGE 4

#define getBit(bit, i) (((bit) >> (i))&1)
#define MASK(x) (1ll << (x))
#define getMask(mask, i) (((mask) >> (X)) & (MASK(BLOCK_EDGE) - 1))

#define SIZEBLOCK 16


#endif