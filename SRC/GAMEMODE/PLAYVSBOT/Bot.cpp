#include "Bot.hpp"

#include "CurrentBlock.hpp"
#include "CurrentBlockController.hpp"
#include "MovementController.hpp"
#include "MonitorForTwoPlayer.hpp"
#include "PlayerWithBot.hpp"

#include <thread>
#include <iostream>

Bot::Bot(int x, int y): X_COORDINATE(x), Y_COORDINATE(y) {
    monitor = new MonitorForTwoPlayer(x, y);
    curBlock = new CurrentBlockController(monitor->getMap());
    movementController = new MovementController(monitor, curBlock);
}

Bot::~Bot() {
    delete monitor; monitor = nullptr;
    delete movementController; monitor = nullptr;
    delete curBlock; monitor = nullptr;
}

void Bot::setGameOver() { monitor->setGameOver(); }
bool Bot::isGameOver() { return monitor->isGameOver(); }

void Bot::start(uint32_t seed, PlayerWithBot* player) {
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();

    std::thread thinking([this](PlayerWithBot* &player) {
        while (!monitor->isGameOver()) {
            sf::Event fakeEvent;
            fakeEvent.type = sf::Event::KeyPressed;
            fakeEvent.key.code = sf::Keyboard::Left;
            if ((rand() % 5) == 0) {
                fakeEvent.type = sf::Event::KeyReleased;
                mtx.lock(); event.push(fakeEvent); mtx.unlock();
                fakeEvent.type = sf::Event::KeyPressed;
                fakeEvent.key.code = sf::Keyboard::Space;
            }
            mtx.lock();
            event.push(fakeEvent);
            mtx.unlock();
            sleep(1);
        }
    }, std::ref(player));
    thinking.detach();
}

void Bot::draw(sf::RenderWindow *window) {
    monitor->draw(window, curBlock->getCurrentBlock());
}

void Bot::handleAddLine(uint8_t nLines) {
    monitor->inforReceiveLineFromCompetitor(nLines);
}

void Bot::update() {
    mtx.lock();
    while (!event.empty()) {
        movementController->processEvents(event.front());
        event.pop();
    }
    mtx.unlock();
    movementController->autoDown();
}