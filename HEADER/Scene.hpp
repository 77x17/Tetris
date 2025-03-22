#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

enum class STATUS_CODE;

class SoundManager;

class Scene {
private:
    sf::Font      font;
    SoundManager *soundManager;
    sf::Texture   menuBackgroundTexture;
    sf::Sprite    menuBackground;
    sf::Clock     overlayTimeout;
    bool          mouseSelect;

public:
    Scene(sf::RenderWindow *window);
    ~Scene();

    // fadeIn: mờ thành rõ (true) | Default* fadeOut: rõ thành mờ (false)
    void drawChangeMenu(sf::RenderWindow *window, bool fadeIn = false);

    STATUS_CODE drawMenu(sf::RenderWindow *window);
    STATUS_CODE drawEscape(sf::RenderWindow *window);
    int waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish);
    STATUS_CODE drawGameOver(sf::RenderWindow *window);
};
