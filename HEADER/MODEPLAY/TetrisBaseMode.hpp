#ifndef TETRISBASEMODE_HPP
#define TETRISBASEMODE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <atomic>

class Scene;

enum class STATUS_CODE;

class TetrisBaseMode {
protected:
    sf::RenderWindow *window;    
    Scene            *scene;
public:
    TetrisBaseMode(sf::RenderWindow*, Scene*);
    ~TetrisBaseMode();
    void loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite);
    virtual STATUS_CODE start() = 0;
};

#endif