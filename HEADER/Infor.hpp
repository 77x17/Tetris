#ifndef INFOR_HPP
#define INFOR_HPP

#include <SFML/Graphics.hpp>

class SoundManager;

class Infor {
private:
    int INFOR_POSITION_X;
    int INFOR_POSITION_Y;
    int INFOR_WIDTH;

    int AUDIO_POSITION_X;
    int AUDIO_POSITION_Y;
    int AUDIO_WIDTH;
    int AUDIO_HEIGHT;
    
    int  nLine;
    int  count;
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
    Infor(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH, int AUDIO_POSITION_X, int AUDIO_POSITION_Y, int AUDIO_WIDTH, int AUDIO_HEIGHT);
    ~Infor();

    void reset();

    void removeLine(uint8_t lines);

    void playSoundRemoveLine(uint8_t lines, bool spin, char typeBlock);
    void drawMessage(sf::RenderWindow *window, const std::string text);
    void drawCombo(sf::RenderWindow *window, const std::string text);
    void drawB2B(sf::RenderWindow *window);
    void drawMissingB2B(sf::RenderWindow *window);
    void drawSpin(sf::RenderWindow *window);
    void drawAudio(sf::RenderWindow *window, const float &volume);
    void draw(sf::RenderWindow *window);

};

#endif