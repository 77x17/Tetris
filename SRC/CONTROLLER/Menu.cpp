#include "Menu.hpp"

#include "Common.hpp"
#include "SoundManager.hpp"

const int BAR_PADDING      = 20;
const int OPTION_PADDING   = 100;
const int SELECTED_PADDING = 90;

const sf::Color MENU_BAR_COLOR = sf::Color(60, 60, 60, 200);
const sf::Color TEXT_COLOR     = sf::Color::White;
const sf::Color SELECTED_COLOR = sf::Color::Yellow;

constexpr float TIME_OUT    = 1.0f;
constexpr float SLIDE_SPEED = 0.05f;

Menu::Menu(sf::RenderWindow *window, const std::vector<std::string> &menuItems, MENU_CODE menuCode) : 
    selected(false), 
    selectedItem(0), 
    mouseSelect(false), 
    mousePosPrev(sf::Vector2i()),
    menuCode(menuCode)
{
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");

    soundManager = new SoundManager();
    soundManager->loadSound("move"    , "ASSETS/sfx/menutap.mp3");
    soundManager->loadSound("selected", "ASSETS/sfx/menu_hit.mp3");

    menuSize  = menuItems.size();
    menuTexts = new sf::Text[menuSize];
    
    switch (menuCode) {
        case MENU_CODE::MAIN: {
            menuBars = new sf::RectangleShape[menuSize];
            currentBarPositionX = new float[menuSize];
            targetBarPositionX  = new float[menuSize];
            
            for (int i = 0; i < menuSize; i++) {
                menuTexts[i].setFont(font);
                menuTexts[i].setString(menuItems[i]);
                menuTexts[i].setCharacterSize(40);
                menuTexts[i].setFillColor(TEXT_COLOR);
                menuTexts[i].setPosition(sf::Vector2f(
                    window->getSize().x / 2.5, 
                    OPTION_PADDING + i * OPTION_PADDING
                ));
                
                currentBarPositionX[i] = window->getSize().x / 2.5;
                targetBarPositionX[i]  = window->getSize().x / 2.5;

                menuBars[i].setSize(sf::Vector2f(
                    window->getSize().x - (window->getSize().x / 2.5 - BAR_PADDING) + SELECTED_PADDING, 
                    OPTION_PADDING - BAR_PADDING
                ));
                menuBars[i].setFillColor(MENU_BAR_COLOR);
                menuBars[i].setPosition(sf::Vector2f(
                    currentBarPositionX[i] - BAR_PADDING, 
                    OPTION_PADDING + i * OPTION_PADDING
                ));
            }

            break;
        }
        case MENU_CODE::PAUSE: 
        case MENU_CODE::GAMEOVER: {
            for (int i = 0; i < menuSize; i++) {
                menuTexts[i].setFont(font);
                menuTexts[i].setString(menuItems[i]);
                menuTexts[i].setCharacterSize(40);
                menuTexts[i].setFillColor(TEXT_COLOR);
                menuTexts[i].setPosition(sf::Vector2f(
                    window->getSize().x / 2 - menuTexts[i].getGlobalBounds().width / 2, 
                    2 * OPTION_PADDING + i * 60
                ));
            }
            
            break;
        }
        default:
            throw std::invalid_argument("[Menu.cpp] cannot find MENU_CODE");
    }
}

Menu::~Menu() {
    delete soundManager;

    delete menuTexts;

    switch (menuCode) {
        case MENU_CODE::MAIN: {
            delete menuBars;
            delete currentBarPositionX;
            delete targetBarPositionX;
            break;
        }
        default:
            // nothing
            break;
    }
}

bool Menu::notSelected() { 
    return not selected; 
}

STATUS_CODE Menu::getSelectedItem() {
    STATUS_CODE result = static_cast<STATUS_CODE>(selectedItem == menuSize - 1 ? -1 : selectedItem);
    
    selected = false;
    selectedItem = 0;
    
    return result; 
}

void Menu::processEvents(sf::RenderWindow *window, sf::Event event) {
    if (event.type == sf::Event::Closed) {
        selectedItem = menuSize - 1;
        
        selected = true;

        soundManager->play("selected");
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up or event.key.code == sf::Keyboard::W) {
            selectedItem = (selectedItem - 1 + menuSize) % menuSize;
            mouseSelect  = false;

            soundManager->play("move");
        } 
        else if (event.key.code == sf::Keyboard::Down or event.key.code == sf::Keyboard::S) {
            selectedItem = (selectedItem + 1           ) % menuSize;
            mouseSelect  = false;

            soundManager->play("move");
        } 
        else if (event.key.code == sf::Keyboard::Space or event.key.code == sf::Keyboard::Enter) {
            selected = true;
            
            soundManager->play("selected");
        }
        else if (menuCode == MENU_CODE::PAUSE and event.key.code == sf::Keyboard::Escape) {
            selectedItem = (int)STATUS_CODE::RESUME;
            selected     = true;
            
            soundManager->play("selected");
        }
    }
    else if (not selected and event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            switch (menuCode) {
                case MENU_CODE::MAIN: {
                    // Lấy vị trí con trỏ trong cửa sổ
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
                    for (int i = 0; i < menuSize; i++) {
                        if (menuBars[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            selectedItem = i;
                            
                            selected = true;
                            
                            soundManager->play("selected");
                            
                            break;
                        }
                    }

                    break;
                }
                case MENU_CODE::PAUSE: 
                case MENU_CODE::GAMEOVER: {
                    // Lấy vị trí con trỏ trong cửa sổ
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
                    for (int i = 0; i < menuSize; i++) {
                        if (menuTexts[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            selectedItem = i;
                            
                            selected = true;
                            
                            soundManager->play("selected");
                            
                            break;
                        }
                    }

                    break;
                }
            }
        }
    }
}

void Menu::update(sf::RenderWindow *window) {
    // Lấy vị trí con trỏ trong cửa sổ
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
    if (mousePos != mousePosPrev) {
        mouseSelect = true;
    }

    mousePosPrev = mousePos;

    switch (menuCode) {
        case MENU_CODE::MAIN: {
            if (mouseSelect) {
                for (int i = 0; i < menuSize; i++) {
                    if (menuBars[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (selectedItem != i) {
                            selectedItem = i;
                            
                            soundManager->play("move");
                        }
                    }
                }
            }

            for (int i = 0; i < menuSize; i++) {
                if (i == selectedItem) {
                    menuTexts[i].setFillColor(SELECTED_COLOR);

                    targetBarPositionX[i] = window->getSize().x / 2.5 - SELECTED_PADDING;
                }
                else {
                    menuTexts[i].setFillColor(TEXT_COLOR);

                    targetBarPositionX[i] = window->getSize().x / 2.5;
                }

                currentBarPositionX[i] += (targetBarPositionX[i] - currentBarPositionX[i]) * SLIDE_SPEED;

                menuTexts[i].setPosition(sf::Vector2f(currentBarPositionX[i], OPTION_PADDING + i * OPTION_PADDING));
        
                menuBars[i].setPosition(sf::Vector2f(currentBarPositionX[i] - BAR_PADDING, OPTION_PADDING + i * OPTION_PADDING));
            }

            break;
        }
        case MENU_CODE::PAUSE:
        case MENU_CODE::GAMEOVER : {
            if (mouseSelect) {
                for (int i = 0; i < menuSize; i++) {
                    if (menuTexts[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (selectedItem != i) {
                            selectedItem = i;
                            
                            soundManager->play("move");
                        }
                    }
                }
            }

            for (int i = 0; i < menuSize; i++) {
                menuTexts[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
            }
            
            break;
        }
    }
}

void Menu::draw(sf::RenderWindow *window) {
    switch (menuCode) {
        case MENU_CODE::MAIN: {
            for (int i = 0; i < menuSize; i++) {
                window->draw(menuBars[i]);
                window->draw(menuTexts[i]);
            }

            break;
        }
        case MENU_CODE::PAUSE   :
        case MENU_CODE::GAMEOVER: {
            for (int i = 0; i < menuSize; i++) {
                window->draw(menuTexts[i]);
            }

            break;
        }
    }
}