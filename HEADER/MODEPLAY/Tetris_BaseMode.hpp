#ifndef TETRISBASEMODE_HPP
#define TETRISBASEMODE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <atomic>

class Scene;

enum class STATUS_CODE;

class Tetris_BaseMode {
protected:
    sf::RenderWindow *window;    
    Scene            *scene;
public:
    Tetris_BaseMode(sf::RenderWindow*, Scene*);
    ~Tetris_BaseMode();
    void loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite);
    virtual STATUS_CODE start() = 0;
};

#endif