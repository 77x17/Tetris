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

bool Monitor::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left and curBlock->moveLeft(map)) {
                if (curBlock->collisionBottom(map)) {
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Right and curBlock->moveRight(map)) {
                if (curBlock->collisionBottom(map)) {
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Down and curBlock->moveDown(map)) {
                if (not curBlock->collisionBottom(map)) {
                    clock.restart();
                }
            } 
            else if (event.key.code == sf::Keyboard::Up and curBlock->rotateLeft(map)) {
                if (curBlock->collisionBottom(map)) {
                    clock.restart();
                }
            } else if (event.key.code == sf::Keyboard::Space) {
                curBlock->hardDrop(map); 
                // if (tetromino.gameOver()) {
                //     tetromino = Tetromino();
                //     board     = Board();
                // }
                clock.restart();
            } else if (event.key.code == sf::Keyboard::C and hold->canHold()) {
                hold->lock();
                curBlock->swap(hold);

                clock.restart();
                
                // if (board.holdPieceShapeType == -1) {
                //     board.setHoldPiece(tetromino);
                //     tetromino.generateTetromino();
                // } else {
                //     tetromino.swapHoldPiece(board.holdPieceShapeType);
                //     clock.restart();
                // }
            }
        } else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Left) {
                moveLeft = false;
            } else if (event.key.code == sf::Keyboard::Right) {
                moveRight = false;
            } else if (event.key.code == sf::Keyboard::Up) {
                rotateFlag = false;
            }
        }
    }
}
