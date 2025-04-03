#ifndef COMMON_MAP_HPP
#define COMMON_MAP_HPP

    #define BLOCK_EDGE 4

    #define getBit(bit, i) (((bit) >> (i))&1)
    #define MASK(x)        (1ll << (x))
    #define FULLMASK(x)    (MASK(x) - 1)

    #define getMask(mask, i) (((mask) >> (i)) & FULLMASK(BLOCK_EDGE))
    #define getLine(shape, i) getMask(shape, (3-(i)) * BLOCK_EDGE)

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

#endif