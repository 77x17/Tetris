#pragma once

#include "Player.hpp"
#include <vector>

enum class MOVE {

};

class Bot : public Player {
private:
    bool isThinking;
    std::vector<MOVE> bestMoves;
    int moveIndex;
    sf::Clock decisionClock;

    void analyzeMove();
    void executeMove();
    
public:
    Bot(int X_COORDINATE, int Y_COORDINATE);
    ~Bot();
    
    void update(); // Cập nhật trạng thái bot theo thời gian
    void draw(sf::RenderWindow *window);

};