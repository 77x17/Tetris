#ifndef INFOR_SURVIVAL_HPP
#define INFOR_SURVIVAL_HPP

#include "Infor_Multiplayer.hpp"

class Infor_Survival: public Infor_Multiplayer {
public:
    Infor_Survival();
    ~Infor_Survival();

    void update (uint8_t lines, bool spin, char typeBlock, bool isAllClear);
    void draw(sf::RenderWindow *window);

};

#endif