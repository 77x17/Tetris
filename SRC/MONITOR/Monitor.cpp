#include "Monitor.hpp"

#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

Monitor::Monitor(sf::RenderWindow* newWindow) {
    window   = newWindow;
    map      = new Map(window);
    hold     = new Hold(window);
    next     = new LinkListBlock(window);
    curBlock = new CurrentBlock(next->updateNext());
    infor    = new Infor(window);
}

Monitor::~Monitor(){
    if (curBlock) {delete curBlock; curBlock = nullptr;}
    if (hold) {delete hold; hold = nullptr;}
    if (next) {delete next; next = nullptr;}
    if (map) {delete map; map = nullptr;}
    if (infor) {delete infor; infor = nullptr;}
}

bool Monitor::moveProcessing() {
    // nodelay(stdscr, TRUE);

    // switch (getch()) {
    //     case ERR:
    //         return false;
    //         break;
    //     case 'a':
    //     case KEY_LEFT:
    //         return curBlock->moveLeft(map);
    //     case 'd':
    //     case KEY_RIGHT:
    //         return curBlock->moveRight(map);
    //     case 's':
    //     case KEY_DOWN:
    //         return curBlock->moveDown(map);
    //     case 'w':
    //     case KEY_UP:
    //         return curBlock->rotateLeft(map);
    //     case 'c':
    //         if (hold->canHold() == false) 
    //             return false;
    //         hold->lock();
    //         hold->erase();
    //         curBlock->erase(map);
    //         curBlock->swap(hold);
    //         hold->draw();
    //         return true;
    //     default:
    //         return false;
    // }
    // return true;
    
    // sf::Event event;
    // while (window.pollEvent(event)) {
    //     if (event.type == sf::Event::Closed) {
    //         window.close();
    //     } else if (event.type == sf::Event::KeyPressed) {
    //         if (event.key.code == sf::Keyboard::Left) {
    //             moveLeft  = true;
    //             moveRight = false;
    //             if (tetromino.move(-BLOCK_SIZE, 0)) {
    //                 if (tetromino.collisionBottom()) {
    //                     clock.restart();
    //                 }
    //             }
    //         } else if (event.key.code == sf::Keyboard::Right) {
    //             moveLeft  = false;
    //             moveRight = true;
    //             if (tetromino.move(+BLOCK_SIZE, 0)) {
    //                 if (tetromino.collisionBottom()) {
    //                     clock.restart();
    //                 }
    //             }
    //         } else if (event.key.code == sf::Keyboard::Down) {
    //             if (tetromino.move(0, +BLOCK_SIZE)) {
    //                 if (not tetromino.collisionBottom()) {
    //                     clock.restart();
    //                 }
    //             }
    //         } else if (event.key.code == sf::Keyboard::Up and rotateFlag == false) {
    //             tetromino.rotate();
    //             if (tetromino.collisionBottom()) {
    //                 clock.restart();
    //             }
    //             rotateFlag = true;
    //         } else if (event.key.code == sf::Keyboard::Space) {
    //             tetromino.hardDrop(); 
    //             if (tetromino.gameOver()) {
    //                 tetromino = Tetromino();
    //                 board     = Board();
    //             }
    //             clock.restart();
    //         } else if (event.key.code == sf::Keyboard::C) {
    //             if (board.holdPieceShapeType == -1) {
    //                 board.setHoldPiece(tetromino);
    //                 tetromino.generateTetromino();
    //             } else {
    //                 tetromino.swapHoldPiece(board.holdPieceShapeType);
    //                 clock.restart();
    //             }
    //         }
    //     } else if (event.type == sf::Event::KeyReleased) {
    //         if (event.key.code == sf::Keyboard::Left) {
    //             moveLeft = false;
    //         } else if (event.key.code == sf::Keyboard::Right) {
    //             moveRight = false;
    //         } else if (event.key.code == sf::Keyboard::Up) {
    //             rotateFlag = false;
    //         }
    //     }
    // }
}
