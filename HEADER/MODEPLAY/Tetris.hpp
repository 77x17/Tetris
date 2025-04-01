#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <atomic>

class Scene;

enum class STATUS_CODE;

class Tetris {
protected:
    sf::RenderWindow *window;    
    Scene            *scene;
public:
    Tetris(sf::RenderWindow*, Scene*);
    ~Tetris();
    void loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite);
    virtual STATUS_CODE start() = 0;
};

#endif