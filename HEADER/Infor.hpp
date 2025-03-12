#ifndef INFOR_HPP
#define INFOR_HPP

#include <SFML/Graphics.hpp>

class Infor {
private:
    uint8_t nLines, nBlock;
public:
    Infor();
    void addLine();
    void addBlock();
    void draw();
};

#endif