#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Scene;

enum class STATUS_CODE;

class Tetris {
private:
    Scene            *scene;
    sf::RenderWindow *window;
    
public:
    Tetris();
    ~Tetris();
    void start();
};

#endif