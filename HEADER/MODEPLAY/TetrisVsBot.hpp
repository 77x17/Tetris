#ifndef TETRIS_VS_BOT_HPP
#define TETRIS_VS_BOT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

#include "TetrisBaseMode.hpp"

class PlayerWithBot;
class Bot;

class TetrisVsBot: public TetrisBaseMode {
private:
    PlayerWithBot* player;
    Bot* competitor;

    std::mt19937 gen;
public:
    TetrisVsBot(sf::RenderWindow*, Scene*);
    ~TetrisVsBot();
    STATUS_CODE start();
};

#endif // TETRIS_VS_BOT_HPP