#ifndef TETRIS_TWO_PLAYER_HPP
#define TETRIS_TWO_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "TetrisBaseMode.hpp"

#include <atomic>

class PlayerWithNetwork;
class Competitor;

class TetrisTowPlayer: public TetrisBaseMode {
private:
    PlayerWithNetwork* player;
    Competitor* competitor;

    bool isHost;

    std::atomic<bool> isFinish;
    void makeConnection();

public:
    TetrisTowPlayer(sf::RenderWindow*, Scene*, bool isHost);
    ~TetrisTowPlayer();
    STATUS_CODE start();
};

#endif