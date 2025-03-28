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