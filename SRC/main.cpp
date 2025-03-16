#include "Common.hpp"
#include "Tetris.hpp"
#include "Monitor.hpp"
#include "Block.hpp"

#include <string.h>

int main(int argc, char **argv) {    
    Tetris tetris;
    if (argc == 2) {
        tetris.startGameTwoPlayer(strcmp(argv[1], "server") == 0);
        return 0;
    }
    tetris.startGameOnePlayer();
    return 0;
}