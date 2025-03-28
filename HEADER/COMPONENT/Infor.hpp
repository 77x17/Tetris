#ifndef INFOR_HPP
#define INFOR_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <mutex>

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
    
    int GARBAGE_POSITION_X;
    int GARBAGE_POSITION_Y;
    int GARBAGE_WIDTH;
    int GARBAGE_HEIGHT;

    sf::Clock     timeout;
    sf::Clock     comboTimeout;
    sf::Clock     B2BMissingTimeout;
    sf::Clock     spinTimeout;
    sf::Clock     garbageSentTimeout;
    std::string   message;
    std::string   combo;
    SoundManager *soundManager;

protected:

    static constexpr float TIME_OUT = 2.0f;

    int  nLine;
    int  count;
    int  B2B;
    int  B2BMissing;
    bool spin;
    bool spinDraw;
    char typeBlock;

    sf::Font      font;

public:
    // Infor(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH, 
    //     int AUDIO_POSITION_X, int AUDIO_POSITION_Y, int AUDIO_WIDTH, int AUDIO_HEIGHT);

    Infor();

    void setPosition(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH, 
        int AUDIO_POSITION_X, int AUDIO_POSITION_Y, int AUDIO_WIDTH, int AUDIO_HEIGHT);

    virtual ~Infor();

    virtual void reset();

    virtual uint8_t removeLine(uint8_t lines);

    void update(uint8_t lines, bool spin, char typeBlock);
    void playSound(uint8_t lines, bool spin, char typeBlock);
    void drawMessage(sf::RenderWindow *window, const std::string text);
    void drawCombo(sf::RenderWindow *window, const std::string text);
    void drawB2B(sf::RenderWindow *window);
    void drawMissingB2B(sf::RenderWindow *window);
    void drawSpin(sf::RenderWindow *window);
    void drawAudio(sf::RenderWindow *window, const float &volume);
    
    virtual void draw(sf::RenderWindow *window);
};

#endif