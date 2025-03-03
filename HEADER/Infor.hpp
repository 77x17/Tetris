#ifndef INFOR_HPP
#define INFOR_HPP

#include <ncurses.h>

class Infor {
private:
    uint8_t nLines, nBlock;
    WINDOW* win;
public:
    Infor(WINDOW* win);
    void addLine();
    void addBlock();
    void draw();
};

#endif