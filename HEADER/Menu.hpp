#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

enum class STATUS_CODE { 
    SINGLEPLAYER,
    MULTIPLAYER,
    OPTION,
         
    PRACTICE,
    VERSUSBOT,

    MULTIPLAYER_SERVER,
    MULTIPLAYER_CLIENT,
    
    RESUME,

    RESTART,
    MENU,

    BACK,

    QUIT
};

enum class MENU_CODE {
    MAIN,
    SINGLEPLAYER,
    MULTIPLAYER,
    PAUSE,
    GAMEOVER
};

class Menu;
class SoundManager;

class Menu {
private:
    sf::Font            font;
    
    sf::Clock           selectedTimeout;

    SoundManager       *soundManager;

    bool                selected;
    int                 selectedItem;
    bool                mouseSelect;
    sf::Vector2i        mousePosPrev;

    int                 menuSize;
    sf::Text           *menuTexts;
    sf::RectangleShape *menuBars;
    float              *currentBarPositionX;
    float              *targetBarPositionX;

    MENU_CODE           menuCode;

    std::vector<Menu *> subMenus;

public:
    Menu(sf::RenderWindow *window, const std::vector<std::string> &menuItems, MENU_CODE menuCode);
    ~Menu();

    bool notSelected();

    STATUS_CODE getSelectedItem();

    void processEvents(sf::RenderWindow *window, sf::Event event);

    void update(sf::RenderWindow *window);

    void draw(sf::RenderWindow *window);

    Menu * getSubMenu(MENU_CODE menuCode);
};
