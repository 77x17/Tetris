#ifndef TETRIS_PRACTICE_HPP
#define TETRIS_PRACTICE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "TetrisBaseMode.hpp"

class Player;

class TetrisPractice: public TetrisBaseMode {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    Player* player;

public:
    TetrisPractice(sf::RenderWindow*, Scene*);
    ~TetrisPractice();
    STATUS_CODE start();
};

#endif