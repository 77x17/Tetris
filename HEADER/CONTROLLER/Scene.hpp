#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

enum class STATUS_CODE;
enum class MENU_CODE;

class SoundManager;
class Menu;
class Option;
class Competitor;

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
    Option       *option;
    Menu         *pauseMenu;
    Menu         *gameOverMenu;
    Menu         *victoryMenu;

public:
    Scene(sf::RenderWindow *window);
    ~Scene();

    // fadeIn: mờ thành rõ (true) | Default* fadeOut: rõ thành mờ (false)
    void drawChangeMenu(sf::RenderWindow *window, bool fadeIn = false);

    void drawMenuBackground(sf::RenderWindow *window);

    STATUS_CODE drawMenu(sf::RenderWindow *window, MENU_CODE menuCode);
    STATUS_CODE drawSubMenu(sf::RenderWindow *window, Menu *subMenu);
    STATUS_CODE drawOption(sf::RenderWindow *window);
    
    STATUS_CODE drawPause(sf::RenderWindow *window);
    STATUS_CODE drawGameOver(sf::RenderWindow *window);
    STATUS_CODE drawVictory(sf::RenderWindow *window);
    
    STATUS_CODE waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish, Competitor* competitor = nullptr);
    
    void drawCountdown(sf::RenderWindow *window, int gridCenterX, int gridCenterY, int otherGridCenterX, int otherGridCenterY);
    void drawScore(sf::RenderWindow *window, int playerScore, int gridCenterX, int gridCenterY, int competitorScore, int otherGridCenterX, int otherGridCenterY);
};
