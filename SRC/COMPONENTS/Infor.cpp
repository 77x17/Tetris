#include "Infor.hpp"

Infor::Infor(WINDOW* _win) {
    win = derwin(_win, 5, 5, 1, 10);
    wrefresh(win);
}

void Infor::addLine() {
    nLines++;
}

void Infor::addBlock() {
    nBlock++;
}

void Infor::draw() {
    
}