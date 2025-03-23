#include "Menu.hpp"

#include "Common.hpp"
#include "SoundManager.hpp"

const int BAR_PADDING      = 20;
const int OPTION_PADDING   = 100;
const int SELECTED_PADDING = 90;

const sf::Color MENU_BAR_COLOR = sf::Color(60, 60, 60);
const sf::Color MENU_TITLE_BAR_COLOR = sf::Color(60, 60, 60, 200);

const sf::Color TEXT_COLOR     = sf::Color::White;
const sf::Color SELECTED_COLOR = sf::Color::Yellow;

constexpr float TIME_OUT          = 1.0f;
constexpr float SLIDE_SPEED       = 0.03f;
constexpr float SELECTED_TIME_OUT = 0.3f;       // Tắt mà bị delay thì Ctrl + F: selectedTimeout.restart()

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
    subMenus.clear();

    // footer
    {
        footerVersion = sf::Text("Version 1.0", font, 15);
        footerVersion.setPosition(
            10, 
            window->getSize().y - footerVersion.getGlobalBounds().height - 10
        );
        footerOwner   = sf::Text("By: HQH - 77x17", font, 15);
        footerOwner  .setPosition(
            window->getSize().x - footerOwner.getGlobalBounds().width - 10, 
            window->getSize().y - footerVersion.getGlobalBounds().height - 10
        );
        footerBar     = sf::RectangleShape(sf::Vector2f(window->getSize().x, footerVersion.getGlobalBounds().height * 2));
        footerBar    .setFillColor(MENU_TITLE_BAR_COLOR);
        footerBar    .setOutlineColor(sf::Color::Black);
        footerBar    .setPosition(0, window->getSize().y - footerVersion.getGlobalBounds().height * 2);
        footerBar    .setOutlineColor(sf::Color::White);
        footerBar    .setOutlineThickness(3);
    }

    // Set title
    switch (menuCode) {
        case MENU_CODE::MAIN: {
            menuTitle = sf::Text("MENU", font, 40);
            menuTitle.setPosition(10, 0);
            menuTitleBar = sf::RectangleShape(sf::Vector2f(window->getSize().x, menuTitle.getGlobalBounds().height * 2));
            menuTitleBar.setFillColor(MENU_TITLE_BAR_COLOR);
            menuTitleBar.setPosition(0, 0);
            menuTitleBar.setOutlineColor(sf::Color::White);
            menuTitleBar.setOutlineThickness(3);

            subMenus = {
                new Menu(window, {
                    "PRACTICE",
                    "VERSUS BOT",
                    "BACK"
                }, MENU_CODE::SINGLEPLAYER),
    
                new Menu(window, {
                    "SERVER",
                    "CLIENT",
                    "BACK"
                }, MENU_CODE::MULTIPLAYER)
            };

            break;
        }
        case MENU_CODE::SINGLEPLAYER: {
            menuTitle = sf::Text("SINGLEPLAYER", font, 40);
            menuTitle.setPosition(10, 0);
            menuTitleBar = sf::RectangleShape(sf::Vector2f(window->getSize().x, 2 * menuTitle.getGlobalBounds().height));
            menuTitleBar.setFillColor(MENU_TITLE_BAR_COLOR);
            menuTitleBar.setPosition(0, 0);
            menuTitleBar.setOutlineColor(sf::Color::White);
            menuTitleBar.setOutlineThickness(3);

            break;
        }
        case MENU_CODE::MULTIPLAYER: {
            menuTitle = sf::Text("MULTIPLAYER", font, 40);
            menuTitle.setPosition(10, 0);
            menuTitleBar = sf::RectangleShape(sf::Vector2f(window->getSize().x, 2 * menuTitle.getGlobalBounds().height));
            menuTitleBar.setFillColor(MENU_TITLE_BAR_COLOR);
            menuTitleBar.setPosition(0, 0);
            menuTitleBar.setOutlineColor(sf::Color::White);
            menuTitleBar.setOutlineThickness(3);

            break;
        }
        case MENU_CODE::PAUSE: {
            menuTitle = sf::Text("PAUSE", font, 50);
            menuTitle.setPosition(window->getSize().x / 2 - menuTitle.getGlobalBounds().width / 2, OPTION_PADDING);
            menuTitleBar = sf::RectangleShape(sf::Vector2f(window->getSize().x, 2 * menuTitle.getGlobalBounds().height));
            menuTitleBar.setFillColor(MENU_TITLE_BAR_COLOR);
            menuTitleBar.setPosition(0, OPTION_PADDING);
            menuTitleBar.setOutlineColor(sf::Color::White);
            menuTitleBar.setOutlineThickness(3);
            
            break;
        }
        case MENU_CODE::GAMEOVER: {
            menuTitle = sf::Text("GAME OVER", font, 50);
            menuTitle.setPosition(window->getSize().x / 2 - menuTitle.getGlobalBounds().width / 2, OPTION_PADDING);
            menuTitleBar = sf::RectangleShape(sf::Vector2f(window->getSize().x, 2 * menuTitle.getGlobalBounds().height));
            menuTitleBar.setFillColor(MENU_TITLE_BAR_COLOR);
            menuTitleBar.setPosition(0, OPTION_PADDING);
            menuTitleBar.setOutlineColor(sf::Color::White);
            menuTitleBar.setOutlineThickness(3);

            break;
        }
        default: 
            throw std::invalid_argument("[Menu.cpp] cannot find MENU_CODE");
    }

    switch (menuCode) {
        case MENU_CODE::MAIN: 
        case MENU_CODE::SINGLEPLAYER: 
        case MENU_CODE::MULTIPLAYER: {
            menuBars = new sf::RectangleShape[menuSize];
            currentBarPositionX = new float[menuSize];
            targetBarPositionX  = new float[menuSize];

            gradient = new sf::VertexArray[menuSize];
            
            for (int i = 0; i < menuSize; i++) {
                {   // Text
                    menuTexts[i] = sf::Text(menuItems[i], font, 40);
                    menuTexts[i].setFillColor(TEXT_COLOR);
                    menuTexts[i].setPosition(sf::Vector2f(
                        window->getSize().x, 
                        OPTION_PADDING + i * OPTION_PADDING
                    ));
                }
                
                {   // Bar - Rectangle
                    currentBarPositionX[i] = window->getSize().x + SELECTED_PADDING;
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

                {   // Disappear
                    gradient[i] = sf::VertexArray(sf::Quads, 4);

                    sf::Vector2f barPos  = menuBars[i].getPosition();
                    sf::Vector2f barSize = menuBars[i].getSize();

                    gradient[i][0].position = sf::Vector2f(barPos.x, barPos.y);
                    gradient[i][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
                    gradient[i][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
                    gradient[i][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
                    
                    sf::Color startColor = MENU_BAR_COLOR;
                    sf::Color endColor   = MENU_BAR_COLOR;     
                    endColor.a = 0; 

                    gradient[i][0].color = startColor;
                    gradient[i][3].color = startColor;
                    gradient[i][1].color = endColor;
                    gradient[i][2].color = endColor;
                }
            }

            break;
        }
        case MENU_CODE::PAUSE: 
        case MENU_CODE::GAMEOVER: {
            menuBars = new sf::RectangleShape[menuSize];

            gradient = new sf::VertexArray[menuSize];

            for (int i = 0; i < menuSize; i++) {
                {   // Text
                    menuTexts[i] = sf::Text(menuItems[i], font, 30);
                    menuTexts[i].setFillColor(TEXT_COLOR);
                    menuTexts[i].setPosition(sf::Vector2f(
                        window->getSize().x / 2 - menuTexts[i].getGlobalBounds().width / 2, 
                        2 * OPTION_PADDING + i * 60
                    ));
                }
                
                {   // Bar - Rectangle
                    menuBars[i].setSize(sf::Vector2f(
                        window->getSize().x / 4,
                        menuTexts[i].getGlobalBounds().height * 2
                    ));
                    menuBars[i].setFillColor(MENU_BAR_COLOR);
                    menuBars[i].setPosition(
                        3 * window->getSize().x / 8, 
                        2 * OPTION_PADDING + i * 60
                    );
                }

                {
                    gradient[i] = sf::VertexArray(sf::TriangleStrip, 8);

                    sf::Vector2f barPos  = menuBars[i].getPosition();
                    sf::Vector2f barSize = menuBars[i].getSize();

                    float leftX   = barPos.x;
                    float midLeftX  = barPos.x + barSize.x / 4;
                    float midRightX = barPos.x + (barSize.x * 3) / 4;
                    float rightX  = barPos.x + barSize.x;

                    // Định vị các điểm
                    gradient[i][0].position = sf::Vector2f(leftX, barPos.y);
                    gradient[i][1].position = sf::Vector2f(leftX, barPos.y + barSize.y);

                    gradient[i][2].position = sf::Vector2f(midLeftX, barPos.y);
                    gradient[i][3].position = sf::Vector2f(midLeftX, barPos.y + barSize.y);

                    gradient[i][4].position = sf::Vector2f(midRightX, barPos.y);
                    gradient[i][5].position = sf::Vector2f(midRightX, barPos.y + barSize.y);

                    gradient[i][6].position = sf::Vector2f(rightX, barPos.y);
                    gradient[i][7].position = sf::Vector2f(rightX, barPos.y + barSize.y);

                    // Màu sắc
                    sf::Color transparentColor = sf::Color(MENU_BAR_COLOR.r, MENU_BAR_COLOR.g, MENU_BAR_COLOR.b, 0);

                    gradient[i][0].color = transparentColor; // Bên trái mờ
                    gradient[i][1].color = transparentColor; // Bên trái mờ

                    gradient[i][2].color = MENU_BAR_COLOR; // Giữa trái rõ
                    gradient[i][3].color = MENU_BAR_COLOR; // Giữa trái rõ

                    gradient[i][4].color = MENU_BAR_COLOR; // Giữa phải rõ
                    gradient[i][5].color = MENU_BAR_COLOR; // Giữa phải rõ

                    gradient[i][6].color = transparentColor; // Bên phải mờ
                    gradient[i][7].color = transparentColor; // Bên phải mờ
                }
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
            for (Menu *subMenu : subMenus) {
                delete subMenu;
            }

            // Không break
        }
        case MENU_CODE::SINGLEPLAYER:
        case MENU_CODE::MULTIPLAYER : {
            delete menuBars;

            delete currentBarPositionX;
            delete targetBarPositionX;

            delete gradient;

            break;
        }
        case MENU_CODE::PAUSE:
        case MENU_CODE::GAMEOVER: {
            delete menuBars;

            delete gradient;

            break;
        }
        default: {
            // nothing
            break;
        }
    }
}

bool Menu::notSelected() { 
    switch (menuCode) {
        case MENU_CODE::MAIN:
        case MENU_CODE::SINGLEPLAYER:
        case MENU_CODE::MULTIPLAYER: {
            if (selected) {
                if (selectedTimeout.getElapsedTime().asSeconds() < SELECTED_TIME_OUT) {
                    return true;
                }
                else {
                    return false;
                }
            } 
            else {
                return true;
            }
        }
        case MENU_CODE::PAUSE:
        case MENU_CODE::GAMEOVER: {
            return not selected; 
        }
        default: 
            throw std::invalid_argument("[Menu.cpp] - notSelected(): MENU_CODE error");
    }

    return true;
}

STATUS_CODE Menu::getSelectedItem() {
    selected = false;
    int cloneSelectedItem = selectedItem;
    selectedItem = 0;

    switch (menuCode) {
        case MENU_CODE::MAIN: {
            switch (cloneSelectedItem) {
                case 0:  return STATUS_CODE::SINGLEPLAYER;
                case 1:  return STATUS_CODE::MULTIPLAYER;
                case 2:  return STATUS_CODE::OPTION;
                case 3:  return STATUS_CODE::QUIT;
                default: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::MAIN) error.");
            }

            break;
        }
        case MENU_CODE::SINGLEPLAYER: {
            switch (cloneSelectedItem) {
                case 0:  return STATUS_CODE::PRACTICE;
                case 1:  return STATUS_CODE::VERSUSBOT;
                case 2:  return STATUS_CODE::BACK;
                default: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::SINGLEPLAYER) error.");
            }

            break;
        }
        case MENU_CODE::MULTIPLAYER: {
            switch (cloneSelectedItem) {
                case 0:  return STATUS_CODE::MULTIPLAYER_SERVER;
                case 1:  return STATUS_CODE::MULTIPLAYER_CLIENT;
                case 2:  return STATUS_CODE::BACK;
                default: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::MULTIPLAYER) error.");
            }

            break;
        }
        case MENU_CODE::PAUSE: {
            switch (cloneSelectedItem) {
                case 0:  return STATUS_CODE::RESUME;
                case 1:  return STATUS_CODE::RESTART;
                case 2:  return STATUS_CODE::MENU;
                case 3:  return STATUS_CODE::QUIT;
                default: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::PAUSE) error.");
            }
            
            break;
        }
        case MENU_CODE::GAMEOVER: {
            switch (cloneSelectedItem) {
                case 0:  return STATUS_CODE::RESTART;
                case 1:  return STATUS_CODE::MENU;
                case 2:  return STATUS_CODE::QUIT;
                default: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::GAMEOVER) error.");
            }
            
            break;
        }
    }

    return STATUS_CODE::QUIT;
}

void Menu::processEvents(sf::RenderWindow *window, sf::Event event) {
    if (event.type == sf::Event::Closed) {
        selectedItem = menuSize - 1;
        
        selected = true;

        soundManager->play("selected");

        selectedTimeout.restart();
    }
    else if (selected) {
        // Do nothing
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

            selectedTimeout.restart();
        }
        else if (event.key.code == sf::Keyboard::Escape and menuCode == MENU_CODE::PAUSE) {
            selectedItem = 0;
            selected     = true;
            
            soundManager->play("selected");

            selectedTimeout.restart();
        }
        else if (event.key.code == sf::Keyboard::Escape and (menuCode == MENU_CODE::SINGLEPLAYER or menuCode == MENU_CODE::MULTIPLAYER)) {
            selectedItem = 2;
            selected     = true;
            
            soundManager->play("selected");

            selectedTimeout.restart();
        }
    }
    else if (not selected and event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Lấy vị trí con trỏ trong cửa sổ
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
            for (int i = 0; i < menuSize; i++) {
                if (menuBars[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    selectedItem = i;
                    
                    selected = true;
                    
                    soundManager->play("selected");
                    
                    selectedTimeout.restart();

                    break;
                }
            }
        }
    }
}

void Menu::update(sf::RenderWindow *window) {
    // Lấy vị trí con trỏ trong cửa sổ
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
    if (mousePos != mousePosPrev) { mouseSelect = true; }
    mousePosPrev = mousePos;

    if (not selected and mouseSelect) {
        for (int i = 0; i < menuSize; i++) {
            if (menuBars[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                if (selectedItem != i) {
                    selectedItem = i;
                    
                    soundManager->play("move");
                }
            }
        }
    }

    switch (menuCode) {
        case MENU_CODE::MAIN: 
        case MENU_CODE::SINGLEPLAYER: 
        case MENU_CODE::MULTIPLAYER: {
            for (int i = 0; i < menuSize; i++) {
                if (not selected) {
                    if (i == selectedItem) {
                        menuTexts[i].setFillColor(SELECTED_COLOR);

                        targetBarPositionX[i] = window->getSize().x / 2.5 - SELECTED_PADDING;
                    }
                    else {
                        menuTexts[i].setFillColor(TEXT_COLOR);

                        targetBarPositionX[i] = window->getSize().x / 2.5;
                    }
                }
                else {
                    if (i == selectedItem) {
                        menuTexts[i].setFillColor(SELECTED_COLOR);

                        targetBarPositionX[i] = window->getSize().x + SELECTED_PADDING;
                    }
                    else {
                        menuTexts[i].setFillColor(TEXT_COLOR);

                        targetBarPositionX[i] = window->getSize().x + SELECTED_PADDING;
                    }
                }

                currentBarPositionX[i] += (targetBarPositionX[i] - currentBarPositionX[i]) * SLIDE_SPEED;

                menuTexts[i].setPosition(sf::Vector2f(currentBarPositionX[i], OPTION_PADDING + i * OPTION_PADDING));
        
                menuBars[i].setPosition(sf::Vector2f(currentBarPositionX[i] - BAR_PADDING, OPTION_PADDING + i * OPTION_PADDING));

                sf::Vector2f barPos  = menuBars[i].getPosition();
                sf::Vector2f barSize = menuBars[i].getSize();

                gradient[i][0].position = sf::Vector2f(barPos.x, barPos.y);
                gradient[i][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
                gradient[i][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
                gradient[i][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
            }

            break;
        }
        case MENU_CODE::PAUSE:
        case MENU_CODE::GAMEOVER : {
            for (int i = 0; i < menuSize; i++) {
                if (i == selectedItem) {
                    menuTexts[i].setFillColor(SELECTED_COLOR);
                    menuTexts[i].setScale(1.1f, 1.1f);
                }
                else {
                    menuTexts[i].setFillColor(TEXT_COLOR);
                    menuTexts[i].setScale(1.0f, 1.0f);
                }

                menuTexts[i].setPosition(sf::Vector2f(
                    window->getSize().x / 2 - menuTexts[i].getGlobalBounds().width / 2, 
                    2 * OPTION_PADDING + i * 60
                ));
            }
            
            break;
        }
    }
}

void Menu::draw(sf::RenderWindow *window) {
    switch (menuCode) {
        case MENU_CODE::MAIN: 
        case MENU_CODE::SINGLEPLAYER: 
        case MENU_CODE::MULTIPLAYER: {
            window->draw(menuTitleBar);
            window->draw(menuTitle);

            window->draw(footerBar);
            window->draw(footerVersion);
            window->draw(footerOwner);

            for (int i = 0; i < menuSize; i++) {
                // window->draw(menuBars[i]);
                window->draw(gradient[i]);
                window->draw(menuTexts[i]);
            }
            
            break;
        }
        case MENU_CODE::PAUSE:
        case MENU_CODE::GAMEOVER: {
            window->draw(menuTitleBar);
            window->draw(menuTitle);
            
            for (int i = 0; i < menuSize; i++) {
                // window->draw(menuBars[i]);
                if (selectedItem == i) {
                    window->draw(gradient[i]);
                }
                window->draw(menuTexts[i]);
            }

            break;
        }
    }
}

Menu * Menu::getSubMenu(MENU_CODE menuCode) {
    for (Menu *subMenu : subMenus) {
        if (subMenu->menuCode == menuCode) {
            return subMenu;
        }
    }

    throw std::invalid_argument("[Menu.cpp] - getSubMenu(): MENU_CODE error");
}