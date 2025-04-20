#ifndef BOT_HPP
#define BOT_HPP

#include <unistd.h>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <SFML/Graphics.hpp>

class Monitor;
class Monitor_VersusBot;
class MovementController_VersusBot;
class CurrentBlockController_Bot;
class Player_VersusBot;

class Bot {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    Monitor_VersusBot* monitor;
    MovementController_VersusBot *movementController;
    CurrentBlockController_Bot* curBlock;

    std::thread brain;
    std::thread handle;

    std::mutex mtx;
    std::atomic<bool> finish;
    std::atomic<bool> pauseGame;
    std::queue<sf::Event> event;

    void update(); // Cập nhật trạng thái bot
    void play(Player_VersusBot* &player); // Bot play

public:
    Bot(int X_COORDINATE, int Y_COORDINATE);
    ~Bot();

    void setGameOver();
    bool isGameOver();

    void setCompetitor(Monitor*);

    void addEvent(const sf::Keyboard::Key &e);

    void start(uint32_t seed, Player_VersusBot*);
    void setTimer();
    void pauseTimer();
    void unPauseTimer();
    void handleAddLine(uint8_t nLines);
    void draw(sf::RenderWindow *window);

};

#endif