#pragma once

#include <SFML/Graphics.hpp>
#include <atomic>

enum class STATUS_CODE {
    QUIT               = -1,

    SINGLEPLAYER       = 0, 
    VERSUSBOT          = 1, 
    MULTIPLAYER_SERVER = 2, 
    MULTIPLAYER_CLIENT = 3, 
    
    RESTART            = 0, 
    MENU               = 1, 
    
    RESUME             = 0
};

class SoundManager;

class Menu {
private:
    sf::Font      font;
    SoundManager *soundManager;
    sf::Texture   menuBackgroundTexture;
    sf::Sprite    menuBackground;
    sf::Clock     overlayTimeout;
    bool          mouseSelect;

public:
    Menu();
    ~Menu();
};
