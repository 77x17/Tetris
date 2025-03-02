#include "Tetris.hpp"

Tetris::Tetris(WINDOW* win):player(win){}

void Tetris::startGameOnePlayer() {
    player.start();
}

// void GameTetris::moveProcessing() {
//     switch (getch()) {
//         case ERR:
//             break;
//         case 'w':
//             cur.rotateLeft();
//         case 'a':
//             cur.moveLeft();
//         case 'd':
//             cur.moveRight();
//         case 's':
//             cur.moveDown();
//         default:
//             break;
//     }
// }