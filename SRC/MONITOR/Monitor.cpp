#include "Monitor.hpp"

Monitor::Monitor(){}

Monitor::Monitor(WINDOW* win): screenPlayer(win){
    nLines = nPieces = 0;
    hold = nullptr; cur = setPiece.get(0); next=setPiece.get(1);
}

bool Monitor::moveProcessing() {
    
}