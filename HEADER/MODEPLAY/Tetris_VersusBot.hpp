#ifndef TETRIS_VS_BOT_HPP
#define TETRIS_VS_BOT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

#include "Tetris_BaseMode.hpp"

class Player_VersusBot;
class Bot;

class Tetris_VersusBot: public Tetris_BaseMode {
private:
    Player_VersusBot* player;
    Bot* competitor;

    std::mt19937 gen;
public:
    Tetris_VersusBot(sf::RenderWindow*, Scene*);
    ~Tetris_VersusBot();
    STATUS_CODE start();
};

#endif // TETRIS_VS_BOT_HPP