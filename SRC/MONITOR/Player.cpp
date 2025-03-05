#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

Player::Player(WINDOW* win):Monitor(win){}

void Player::start() {
    std::atomic<int> timeout{500};
    std::atomic<bool> isRunning{true};
    std::mutex mtx;
    
    next->draw(); 
    curBlock->draw(map);

    auto moveDown = [&]() {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout.load()));
            
            mtx.lock();
            
            if (curBlock && !curBlock->moveDown(map)) {
                curBlock->put(map);
                delete curBlock;
                curBlock = next->updateNext();
            }
            if (curBlock == nullptr) isRunning = false;
            mtx.unlock();
            
        }
    };

    std::thread t(moveDown);
    t.detach();

    while (isRunning) {
        mtx.lock();
        if (moveProcessing())
            timeout = 500;
        mtx.unlock();
    }
}