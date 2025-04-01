#ifndef INFOR_MULTIPLAYER_HPP
#define INFOR_MULTIPLAYER_HPP

#include "Infor.hpp"

class Infor_Multiplayer: public Infor {
private:
    int GARBAGE_POSITION_X;
    int GARBAGE_POSITION_Y;
    int GARBAGE_WIDTH;
    int GARBAGE_HEIGHT;

    sf::Clock garbageSentTimeout;

    std::mutex mtx;
    uint64_t nLinesAdd;

    int garbageSent;
    int getGarbage(int lines, bool spin, int B2B, int count);
public:
    Infor_Multiplayer();
    void setPosition(int INFOR_POSITION_X, int INFOR_POSITION_Y, int INFOR_WIDTH, 
        int GARBAGE_POSITION_X, int GARBAGE_POSITION_Y, int GARBAGE_WIDTH, int GARBAGE_HEIGHT);
    
    virtual ~Infor_Multiplayer();
        
    void reset();
    uint8_t removeLine(uint8_t lines);
    void addLine(uint8_t lines);
    uint64_t  getAndRemoveLineAdd();

    void drawGarbage(sf::RenderWindow *window);
    void drawGarbageSent(sf::RenderWindow *window);

    virtual void draw(sf::RenderWindow *window);
};

#endif // INFORFORNETWORK_HPP