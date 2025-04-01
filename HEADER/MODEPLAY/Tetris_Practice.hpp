#ifndef TETRIS_PRACTICE_HPP
#define TETRIS_PRACTICE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Tetris_BaseMode.hpp"

class Player;

class Tetris_Practice: public Tetris_BaseMode {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    Player* player;

public:
    Tetris_Practice(sf::RenderWindow*, Scene*);
    ~Tetris_Practice();
    STATUS_CODE start();
};

#endif