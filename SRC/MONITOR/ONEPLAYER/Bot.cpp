#include "Bot.hpp"

#include "CurrentBlock.hpp"

Bot::Bot(int X_COORDINATE, int Y_COORDINATE) :
    Player(X_COORDINATE, Y_COORDINATE), 
    isThinking(false), 
    moveIndex(0) {}

Bot::~Bot() {}

void Bot::draw(sf::RenderWindow *window) {
    Monitor::draw(window);
        
    curBlock->draw(window, map);
}