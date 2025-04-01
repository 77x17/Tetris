#ifndef TETRIS_SURVIVAL_HPP
#define TETRIS_SURVIVAL_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Tetris.hpp"

class Player_Survival;

class Tetris_Survival: public Tetris {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    Player_Survival* player;

    sf::Clock timer;

public:
    Tetris_Survival(sf::RenderWindow*, Scene*);
    ~Tetris_Survival();
    STATUS_CODE start();
};

#endif