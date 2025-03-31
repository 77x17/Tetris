#include "Bot.hpp"

#include "CurrentBlock.hpp"
#include "CurrentBlockController.hpp"
#include "MovementControllerWithBot.hpp"
#include "MonitorForBot.hpp"
#include "PlayerWithBot.hpp"

#include <thread>
#include <iostream>

Bot::Bot(int x, int y): X_COORDINATE(x), Y_COORDINATE(y) {
    monitor = new MonitorForBot(x, y);
    curBlock = new CurrentBlockController(monitor->getMap());
    movementController = new MovementControllerWithBot(monitor, curBlock);
}

Bot::~Bot() {
    delete monitor; monitor = nullptr;
    delete movementController; monitor = nullptr;
    delete curBlock; monitor = nullptr;
}

void Bot::setGameOver() { monitor->setGameOver(); }
bool Bot::isGameOver() { return monitor->isGameOver(); }

void Bot::addEvent() {
    sf::Event fakeEvent;
    fakeEvent.type = sf::Event::KeyPressed;
    fakeEvent.key.code = sf::Keyboard::Left;
    mtx.lock();
    event.push(fakeEvent);
    mtx.unlock();
}

void Bot::start(uint32_t seed, PlayerWithBot* player) {
    player->setCompetitor(monitor);
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();

    std::thread thinking([this](PlayerWithBot* &player) {
        while (!monitor->isGameOver()) {
            addEvent();
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

void Bot::setCompetitor(Monitor* mon) {
    dynamic_cast<MovementControllerWithBot*>(movementController)->setCompetitor(mon);
}