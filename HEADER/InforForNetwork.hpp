#ifndef INFORFORNETWORK_HPP
#define INFORFORNETWORK_HPP

#include "Infor.hpp"

class InforForNetwork: public Infor {
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

    InforForNetwork(int GARBAGE_POSITION_X, int GARBAGE_POSITION_Y, int GARBAGE_WIDTH, int GARBAGE_HEIGHT);

    void drawGarbage(sf::RenderWindow *window);
    void drawGarbageSent(sf::RenderWindow *window);

    void draw(sf::RenderWindow *window);
    void compress(sf::Packet &packet);
};

#endif // INFORFORNETWORK_HPP