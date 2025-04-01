#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Scene;

enum class STATUS_CODE;

class Game {
private:
    Scene            *scene;
    sf::RenderWindow *window;
    
public:
    Game();
    ~Game();
    void start();
};

#endif