#ifndef BOT_HPP
#define BOT_HPP

#include <unistd.h>
#include <SFML/Graphics.hpp>

class MonitorForTwoPlayer;
class MovementController;
class CurrentBlockController;
class PlayerWithBot;

class Bot {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    MonitorForTwoPlayer* monitor;
    MovementController *movementController;
    CurrentBlockController* curBlock;
public:
    Bot(int X_COORDINATE, int Y_COORDINATE);
    ~Bot();
    
    void setGameOver();
    bool isGameOver();

    void start(uint32_t seed, PlayerWithBot*);

    void handleAddLine(uint8_t nLines);

    void update(); // Cập nhật trạng thái bot theo thời gian
    void draw(sf::RenderWindow *window);

};

#endif