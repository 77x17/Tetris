#ifndef INFOR_HPP
#define INFOR_HPP

#include <SFML/Graphics.hpp>

class SoundManager;

class Infor {
private:
    int INFOR_POSITION_X;
    int INFOR_POSITION_Y;
    int INFOR_WIDTH;
    
    int  nLine;
    int  count;
    int  add;
    int  B2B;
    int  B2BMissing;
    bool spin;
    char typeBlock;

    sf::Clock     timeout;
    sf::Clock     comboTimeout;
    sf::Clock     B2BMissingTimeout;
    sf::Clock     spinTimeout;
    sf::Font      font;
    std::string   message;
    std::string   combo;
    SoundManager *soundManager;

public:
    Infor(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH);
    ~Infor();

    void reset();

    void addLine(uint8_t lines, bool spin, char typeBlock);
    void drawMessage(sf::RenderWindow *window, const std::string text);
    void drawCombo(sf::RenderWindow *window, const std::string text);
    void drawB2B(sf::RenderWindow *window);
    void drawMissingB2B(sf::RenderWindow *window);
    void drawSpin(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);

};

#endif