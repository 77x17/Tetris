#ifndef TETRIS_TWO_PLAYER_HPP
#define TETRIS_TWO_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "TetrisBaseMode.hpp"

#include <atomic>

class PlayerWithNetwork;
class Competitor;

class TetrisTwoPlayer: public TetrisBaseMode {
private:
    PlayerWithNetwork* player;
    Competitor* competitor;

    bool isHost;

    std::atomic<bool> isFinish;
    void makeConnection();

public:
    TetrisTwoPlayer(sf::RenderWindow*, Scene*, bool isHost);
    ~TetrisTwoPlayer();
    STATUS_CODE start();
};

#endif