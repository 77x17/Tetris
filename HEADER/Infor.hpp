#ifndef INFOR_HPP
#define INFOR_HPP

#include <SFML/Graphics.hpp>

class Infor {
private:
    int INFOR_POSITION_X = 50;
    int INFOR_POSITION_Y = 100;
    int INFOR_WIDTH;
    
    int         nLine;
    int         count;
    int         add;
    int         B2B;
    int         B2BMissing;

    sf::Clock   timeout;
    sf::Font    font;
    std::string message;
    std::string combo;

public:
    Infor(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH);
    void addLine(int lines);
    void drawMessage(sf::RenderWindow *window, const std::string text);
    void drawCombo(sf::RenderWindow *window, const std::string text);
    void drawB2B(sf::RenderWindow *window);
    void drawMissingB2B(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);

};

#endif