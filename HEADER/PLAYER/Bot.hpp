#ifndef BOT_HPP
#define BOT_HPP

#include <unistd.h>
#include <mutex>
#include <queue>
#include <SFML/Graphics.hpp>

class Monitor;
class MonitorForTwoPlayer;
class MovementControllerWithBot;
class CurrentBlockController;
class PlayerWithBot;

class Bot {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    MonitorForTwoPlayer* monitor;
    MovementControllerWithBot *movementController;
    CurrentBlockController* curBlock;

    std::mutex mtx;
    std::queue<sf::Event> event;

public:
    Bot(int X_COORDINATE, int Y_COORDINATE);
    ~Bot();

    void setGameOver();
    bool isGameOver();

    void setCompetitor(Monitor*);
    
    void addEvent();

    void start(uint32_t seed, PlayerWithBot*);

    void handleAddLine(uint8_t nLines);

    void update(); // Cập nhật trạng thái bot theo thời gian
    void draw(sf::RenderWindow *window);

};

#endif