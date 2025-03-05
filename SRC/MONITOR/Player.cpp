#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

Player::Player(WINDOW* win):Monitor(win){}

void Player::start() {
    std::atomic<int> timeout{500};
    std::atomic<bool> isRunning{true};
    std::mutex mtx;
    
    hold->unlock();
    next->draw(); 
    curBlock->draw(map, true);

    auto moveDown = [&]() {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout.load()));
            
            mtx.lock();
            if (curBlock && !curBlock->moveDown(map)) {
                hold->unlock();
                curBlock->put(map);
                curBlock->setter(next->updateNext());
            }
            mtx.unlock();
        }
    };

    std::thread t(moveDown);
    t.detach();

    while (isRunning) {
        mtx.lock();
        if (moveProcessing()) {
            if (curBlock->isEmpty()) 
                curBlock->setter(next->updateNext());
            timeout = 500;
        }
        mtx.unlock();
    }
}