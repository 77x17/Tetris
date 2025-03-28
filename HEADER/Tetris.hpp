#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player;
class PlayerWithNetwork;
class Competitor;
class NetworkManager;
class Scene;

enum class STATUS_CODE;

class Tetris {
private:
    Scene            *scene;
    sf::RenderWindow *window;

    bool notFocus(sf::RenderWindow *window);
    
public:
    Tetris();
    ~Tetris();
    void start();
    void loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite);
    STATUS_CODE startGameVersusBot();
    STATUS_CODE startGameOnePlayer();
};

#endif