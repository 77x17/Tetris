#ifndef INFOR_HPP
#define INFOR_HPP

#include "Common.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <mutex>

class SoundManager;

const int   FONT_SIZE = Common::BLOCK_SIZE;

const int MESSAGE_FONT_SIZE = FONT_SIZE;
const int   MESSAGE_PADDING = FONT_SIZE;

const int   COMBO_FONT_SIZE = FONT_SIZE * 2;
const int     COMBO_PADDING = FONT_SIZE * 3;
const int   N_COMBO_PADDING = FONT_SIZE * 2 + FONT_SIZE / 2;

const int     B2B_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
const int       B2B_PADDING = FONT_SIZE * 2;

const int    SPIN_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;

const int     PPS_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
const int       PPS_PADDING = FONT_SIZE * 7;

const int   LEVEL_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
const int     LINES_PADDING = FONT_SIZE * 9;

const int   TIMER_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
const int     TIMER_PADDING = FONT_SIZE * 11;

class Infor {
private:
    int GARBAGE_POSITION_X;
    int GARBAGE_POSITION_Y;
    int GARBAGE_WIDTH;
    int GARBAGE_HEIGHT;

    sf::Clock     timeout;
    sf::Clock     comboTimeout;
    sf::Clock     B2BMissingTimeout;
    sf::Clock     spinTimeout;
    sf::Clock     garbageSentTimeout;
    sf::Clock     allClearTimeout;
    std::string   message;
    std::string   combo;
    SoundManager *soundManager;

    
    
protected:
    int INFOR_POSITION_X;
    int INFOR_POSITION_Y;
    int INFOR_WIDTH;
    
    sf::Clock     runningTime;
    sf::Time      lastElapsed;
    bool          startTimer;
    bool          isAllClear;

    static constexpr float TIME_OUT = 2.0f;

    int  nLine;
    int  piece;
    int  count;
    int  B2B;
    int  B2BMissing;
    bool spin;
    bool spinDraw;
    char typeBlock;

    sf::Font      font;

public:
    Infor();

    void setPosition(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH);

    void setTimer();
    void pauseTimer();
    void unPauseTimer();

    virtual ~Infor();

    virtual void reset();

    virtual uint8_t removeLine(uint8_t lines);

    virtual void update(uint8_t lines, bool spin, char typeBlock, bool isAllClear);
    void playSound     (uint8_t lines, bool spin, char typeBlock, bool isAllClear);
    void drawMessage   (sf::RenderWindow *window, const std::string text);
    void drawCombo     (sf::RenderWindow *window, const std::string text);
    void drawB2B       (sf::RenderWindow *window);
    void drawMissingB2B(sf::RenderWindow *window);
    void drawSpin      (sf::RenderWindow *window);
    void drawPPS       (sf::RenderWindow *window);
    void drawLevel     (sf::RenderWindow *window);
    void drawTimer     (sf::RenderWindow *window);
    void drawAllClear  (sf::RenderWindow *window);

    int getSecond();

    virtual void draw(sf::RenderWindow *window, int mode);
};

#endif