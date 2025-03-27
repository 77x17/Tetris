#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

enum class STATUS_CODE;

class SoundManager;
class Menu;

class Scene {
private:
    sf::Font      font;
    SoundManager *soundManager;

    float         menuBackgroundX;
    sf::Clock     menuBackgroundClock;
    sf::Texture   menuBackgroundTexture;
    sf::Sprite    menuBackground;
    
    sf::Clock     overlayTimeout;

    Menu         *mainMenu;
    Menu         *pauseMenu;
    Menu         *gameOverMenu;

    bool          notFocus(sf::RenderWindow *window);
public:
    Scene(sf::RenderWindow *window);
    ~Scene();

    // fadeIn: mờ thành rõ (true) | Default* fadeOut: rõ thành mờ (false)
    void drawChangeMenu(sf::RenderWindow *window, bool fadeIn = false);

    void drawMenuBackground(sf::RenderWindow *window);

    STATUS_CODE drawMenu(sf::RenderWindow *window);
    STATUS_CODE drawSubMenu(sf::RenderWindow *window, Menu *subMenu);
    
    STATUS_CODE drawPause(sf::RenderWindow *window);
    STATUS_CODE drawGameOver(sf::RenderWindow *window);
    
    int waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish);
    
    void drawCountdown(sf::RenderWindow *window, int gridCenterX, int gridCenterY, int otherGridCenterX, int otherGridCenterY);
};
