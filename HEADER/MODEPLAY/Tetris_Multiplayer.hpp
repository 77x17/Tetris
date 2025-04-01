#ifndef TETRIS_TWO_PLAYER_HPP
#define TETRIS_TWO_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Tetris.hpp"

#include <atomic>

class Player_Multiplayer;
class Competitor;

class Tetris_Multiplayer: public Tetris {
private:
    Player_Multiplayer* player;
    Competitor* competitor;

    bool isHost;

    std::atomic<bool> isFinish;
    void makeConnection();

public:
    Tetris_Multiplayer(sf::RenderWindow*, Scene*, bool isHost);
    ~Tetris_Multiplayer();
    STATUS_CODE start();
};

#endif