#ifndef TETRIS_ONE_PLAYER_HPP
#define TETRIS_ONE_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "TetrisBaseMode.hpp"

class Player;

class TetrisOnePlayer: public TetrisBaseMode {
private:
    Player* player;

public:
    TetrisOnePlayer(sf::RenderWindow*, Scene*);
    ~TetrisOnePlayer();
    STATUS_CODE start();
};

#endif