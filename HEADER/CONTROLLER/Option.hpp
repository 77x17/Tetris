#pragma once

#include <SFML/Graphics.hpp>

class SoundManager;
class KeyConfiguration;
enum class STATUS_CODE;

class Option {
    private:
        sf::Font            font;
        
        KeyConfiguration   *keyConfiguration;
    
        sf::Clock           selectedTimeout;
    
        SoundManager       *soundManager;
    
        bool                optionSelected;
        bool                audioSelected;
        int                 optionSelectedIndex;
        int                 optionSelectedItem;
        std::string         optionKeyStrings[2];
        sf::Text            optionKeyTexts[2];
    
        bool                optionWaitForKey;
        bool                optionWaitForReleaseKey;
    
        bool                selected;
        int                 selectedItem;
        bool                mouseSelect;
        sf::Vector2i        mousePosPrev;
    
        int                 menuSize;
        sf::Text           *menuTexts;
        
        sf::Text            menuTitle;
        sf::RectangleShape  menuTitleBar;
    
        sf::Text            footerVersion;
        sf::Text            footerOwner;
        sf::RectangleShape  footerBar;
    
        sf::RectangleShape *menuBars;
        sf::VertexArray    *gradient;
        float              *originalPositionX;
        float              *currentBarPositionX;
        float              *targetBarPositionX;
    
    public:
        Option(sf::RenderWindow *window, const std::vector<std::string> &menuItems);
        ~Option();
    
        bool notSelected();
    
        STATUS_CODE getSelectedItem();
    
        void processEvents(sf::RenderWindow *window, sf::Event event);
    
        void update(sf::RenderWindow *window);
    
        void draw(sf::RenderWindow *window);
    };
    