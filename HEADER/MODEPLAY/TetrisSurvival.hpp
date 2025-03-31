#ifndef TETRIS_SURVIVAL_HPP
#define TETRIS_SURVIVAL_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "TetrisBaseMode.hpp"

class PlayerSurvival;

class TetrisSurvival: public TetrisBaseMode {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    PlayerSurvival* player;

    sf::Clock timer;

public:
    TetrisSurvival(sf::RenderWindow*, Scene*);
    ~TetrisSurvival();
    STATUS_CODE start();
};

#endif