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

    sf::Clock     runningTime;
    sf::Time      lastElapsed;
    bool          startTimer;

    bool          isAllClear;

protected:

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

    virtual void draw(sf::RenderWindow *window);
};

#endif