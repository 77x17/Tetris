#ifndef INFOR_HPP
#define INFOR_HPP

#include <SFML/Graphics.hpp>

class Infor {
private:
    int INFOR_POSITION_X = 50;
    int INFOR_POSITION_Y = 100;
    
    int         nLine;
    int         count;
    int         add;
    bool        drawAble;
    sf::Clock   timeout;
    sf::Font    font;
    std::string message;
    std::string combo;

public:
    Infor(int INFOR_POSITION_X, int INFOR_POSITION_Y);
    void addLine(int lines);
    void drawMessage(sf::RenderWindow *window, const std::string text);
    void drawCombo(sf::RenderWindow *window, const std::string text);
    void draw(sf::RenderWindow *window);

};

#endif