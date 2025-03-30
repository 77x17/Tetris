#include "Option.hpp"

#include "Menu.hpp"
#include "SoundManager.hpp"
#include "KeyConfiguration.hpp"

const int BAR_PADDING      = 20;
const int OPTION_PADDING   = 100;
const int SELECTED_PADDING = 90;

const sf::Color MENU_BAR_COLOR        = sf::Color(60, 60, 60);
const sf::Color MENU_OPTION_BAR_COLOR = sf::Color(35, 35, 35);
const sf::Color MENU_TITLE_BAR_COLOR  = sf::Color(30, 30, 30, 200);

const sf::Color TEXT_COLOR     = sf::Color::White;
const sf::Color SELECTED_COLOR = sf::Color::Yellow;

constexpr float TIME_OUT          = 1.0f;
constexpr float SLIDE_SPEED       = 0.195f;
constexpr float SELECTED_TIME_OUT = 0.3f;       // Tắt mà bị delay thì Ctrl + F: selectedTimeout.restart()

Option::Option(sf::RenderWindow *window, const std::vector<std::string> &menuItems) :
    optionSelected(false),
    audioSelected(false),
    optionWaitForKey(false),
    selected(false), 
    selectedItem(0), 
    mouseSelect(false), 
    mousePosPrev(sf::Vector2i())
{
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");

    keyConfiguration = new KeyConfiguration("ASSETS/keyBindings.txt");

    soundManager = new SoundManager();
    soundManager->loadSound("move"    , "ASSETS/sfx/menutap.mp3");
    soundManager->loadSound("selected", "ASSETS/sfx/menu_hit.mp3");
    soundManager->loadSound("scroll", "ASSETS/sfx/scroll.mp3");

    menuSize  = menuItems.size();   
    menuTexts = new sf::Text[menuSize];

    {   // footer
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

    {   // title
        menuTitle = sf::Text("OPTION", font, 40);
        menuTitle.setPosition(10, 0);
        menuTitleBar = sf::RectangleShape(sf::Vector2f(window->getSize().x, 2 * menuTitle.getGlobalBounds().height));
        menuTitleBar.setFillColor(MENU_TITLE_BAR_COLOR);
        menuTitleBar.setPosition(0, 0);
        menuTitleBar.setOutlineColor(sf::Color::White);
        menuTitleBar.setOutlineThickness(3);
    }

    menuBars = new sf::RectangleShape[menuSize];
    gradient = new sf::VertexArray[menuSize];
    originalPositionX   = new float[menuSize];
    currentBarPositionX = new float[menuSize];
    targetBarPositionX  = new float[menuSize];
    
    // Back button
    {   // Text
        menuTexts[menuSize - 1] = sf::Text(menuItems[menuSize - 1], font, 40);
        menuTexts[menuSize - 1].setFillColor(TEXT_COLOR);
        menuTexts[menuSize - 1].setPosition(sf::Vector2f(
            window->getSize().x, 
            OPTION_PADDING
        ));
        // Bar - Rectangle
        originalPositionX[menuSize - 1]   = window->getSize().x / 1.25;
        currentBarPositionX[menuSize - 1] = window->getSize().x;
        targetBarPositionX[menuSize - 1]  = originalPositionX[menuSize - 1];
        
        menuBars[menuSize - 1].setSize(sf::Vector2f(
            window->getSize().x - (originalPositionX[menuSize - 1] - BAR_PADDING), 
            OPTION_PADDING - BAR_PADDING
        ));
        menuBars[menuSize - 1].setFillColor(MENU_BAR_COLOR);
        menuBars[menuSize - 1].setPosition(sf::Vector2f(
            currentBarPositionX[menuSize - 1] - BAR_PADDING, 
            OPTION_PADDING
        ));
        // Disappear
        gradient[menuSize - 1] = sf::VertexArray(sf::Quads, 4);

        sf::Vector2f barPos  = menuBars[menuSize - 1].getPosition();
        sf::Vector2f barSize = menuBars[menuSize - 1].getSize();

        gradient[menuSize - 1][0].position = sf::Vector2f(barPos.x, barPos.y);
        gradient[menuSize - 1][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
        gradient[menuSize - 1][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
        gradient[menuSize - 1][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
        
        sf::Color startColor = MENU_BAR_COLOR;
        sf::Color endColor   = MENU_BAR_COLOR;     
        endColor.a = 0; 

        gradient[menuSize - 1][0].color = startColor;
        gradient[menuSize - 1][3].color = startColor;
        gradient[menuSize - 1][1].color = endColor;
        gradient[menuSize - 1][2].color = endColor;
    }

    // Controls button
    {   // Text
        menuTexts[0] = sf::Text(menuItems[0], font, 40);
        menuTexts[0].setFillColor(TEXT_COLOR);
        menuTexts[0].setPosition(sf::Vector2f(
            window->getSize().x, 
            OPTION_PADDING
        ));
        // Bar - Rectangle
        originalPositionX[0]   = OPTION_PADDING;
        currentBarPositionX[0] = window->getSize().x;
        targetBarPositionX[0]  = originalPositionX[0];
        
        menuBars[0].setSize(sf::Vector2f(
            window->getSize().x - (originalPositionX[0] - BAR_PADDING) - menuBars[menuSize - 1].getSize().x - OPTION_PADDING, 
            // window->getSize().y - (menuTexts[0].getPosition().y) - footerBar.getSize().y - (OPTION_PADDING - menuTitleBar.getSize().y)
            OPTION_PADDING - BAR_PADDING
        ));
        sf::Color blurColor = MENU_BAR_COLOR;
        blurColor.a = 200;
        menuBars[0].setFillColor(blurColor);
        menuBars[0].setPosition(sf::Vector2f(
            currentBarPositionX[0] - BAR_PADDING, 
            OPTION_PADDING
        ));
    }

    // Key - Controls
    int controlHeight = window->getSize().y - (2 * OPTION_PADDING) - footerBar.getSize().y - (OPTION_PADDING - menuTitleBar.getSize().y);
    int keyBarPadding = controlHeight / 8;
    for (int i = 1; i < menuSize - 4; i++) {
        std::pair<std::string, std::string> keys = keyConfiguration->getKey(static_cast<EVENT>(i - 1));
        
        while (keys.first .size() != 10) keys.first  += ' ';
        while (keys.second.size() != 10) keys.second += ' ';

        std::string addingKey = " [ " + keys.first + " | " + keys.second + " ]";
        menuTexts[i] = sf::Text(menuItems[i] + addingKey, font, 20);
        menuTexts[i].setFillColor(TEXT_COLOR);
        menuTexts[i].setPosition(sf::Vector2f(
            window->getSize().x, 
            2 * OPTION_PADDING + keyBarPadding * (i - 1) + keyBarPadding / 4
        ));
        // Bar - Rectangle
        originalPositionX[i]   = OPTION_PADDING;
        currentBarPositionX[i] = window->getSize().x;
        targetBarPositionX[i]  = originalPositionX[i];
        
        menuBars[i].setSize(sf::Vector2f(
            menuBars[0].getSize().x, 
            keyBarPadding
        ));
        sf::Color blurColor = MENU_BAR_COLOR;
        blurColor.a = 200;
        menuBars[i].setFillColor(blurColor);
        menuBars[i].setPosition(sf::Vector2f(
            currentBarPositionX[i] - BAR_PADDING, 
            2 * OPTION_PADDING + keyBarPadding * (i - 1)
        ));
    }

    // Audio button
    {   // Text
        menuTexts[menuSize - 2] = sf::Text(menuItems[menuSize - 2], font, 40);
        menuTexts[menuSize - 2].setFillColor(TEXT_COLOR);
        menuTexts[menuSize - 2].setPosition(sf::Vector2f(
            window->getSize().x, 
            OPTION_PADDING + OPTION_PADDING
        ));
        // Bar - Rectangle
        originalPositionX[menuSize - 2]   = window->getSize().x / 1.25;
        currentBarPositionX[menuSize - 2] = window->getSize().x;
        targetBarPositionX[menuSize - 2]  = originalPositionX[menuSize - 2];
        
        menuBars[menuSize - 2].setSize(sf::Vector2f(
            window->getSize().x - (originalPositionX[menuSize - 1] - BAR_PADDING), 
            OPTION_PADDING - BAR_PADDING
        ));
        menuBars[menuSize - 2].setFillColor(MENU_BAR_COLOR);
        menuBars[menuSize - 2].setPosition(sf::Vector2f(
            currentBarPositionX[menuSize - 2] - BAR_PADDING, 
            OPTION_PADDING + OPTION_PADDING
        ));
        // Disappear
        gradient[menuSize - 2] = sf::VertexArray(sf::Quads, 4);

        sf::Vector2f barPos  = menuBars[menuSize - 2].getPosition();
        sf::Vector2f barSize = menuBars[menuSize - 2].getSize();

        gradient[menuSize - 2][0].position = sf::Vector2f(barPos.x, barPos.y);
        gradient[menuSize - 2][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
        gradient[menuSize - 2][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
        gradient[menuSize - 2][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
        
        sf::Color startColor = MENU_BAR_COLOR;
        sf::Color endColor   = MENU_BAR_COLOR;     
        endColor.a = 0; 

        gradient[menuSize - 2][0].color = startColor;
        gradient[menuSize - 2][3].color = startColor;
        gradient[menuSize - 2][1].color = endColor;
        gradient[menuSize - 2][2].color = endColor;
    }

    // SFX - Audio
    int audioHeight = window->getSize().y - (3 * OPTION_PADDING) - footerBar.getSize().y - (OPTION_PADDING - menuTitleBar.getSize().y) + BAR_PADDING;
    {   // Text
        menuTexts[menuSize - 4] = sf::Text(menuItems[menuSize - 4], font, 20);
        menuTexts[menuSize - 4].setFillColor(TEXT_COLOR);
        menuTexts[menuSize - 4].setPosition(sf::Vector2f(
            window->getSize().x,
            menuTexts[menuSize - 5].getPosition().y         // HOLD position y
        ));
        // Bar - Rectangle
        originalPositionX[menuSize - 4]   = window->getSize().x / 1.25;
        currentBarPositionX[menuSize - 4] = window->getSize().x;
        targetBarPositionX[menuSize - 4]  = originalPositionX[menuSize - 4];
        
        menuBars[menuSize - 4].setSize(sf::Vector2f(
            (window->getSize().x - (originalPositionX[menuSize - 1] - BAR_PADDING)) / 2, 
            audioHeight
        ));
        menuBars[menuSize - 4].setFillColor(MENU_BAR_COLOR);
        menuBars[menuSize - 4].setPosition(sf::Vector2f(
            currentBarPositionX[menuSize - 4] - BAR_PADDING, 
            OPTION_PADDING + OPTION_PADDING * 2 - BAR_PADDING
        ));
        // Disappear
        gradient[menuSize - 4] = sf::VertexArray(sf::Quads, 4);

        sf::Vector2f barPos  = menuBars[menuSize - 4].getPosition();
        sf::Vector2f barSize = menuBars[menuSize - 4].getSize();

        gradient[menuSize - 4][0].position = sf::Vector2f(barPos.x, barPos.y);
        gradient[menuSize - 4][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
        gradient[menuSize - 4][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
        gradient[menuSize - 4][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
        
        sf::Color startColor = MENU_BAR_COLOR;
        sf::Color endColor   = MENU_BAR_COLOR;     
        endColor.a = 255 / 2; 

        gradient[menuSize - 4][0].color = startColor;
        gradient[menuSize - 4][3].color = startColor;
        gradient[menuSize - 4][1].color = endColor;
        gradient[menuSize - 4][2].color = endColor;
    }

    // Music - Audio
    {     
        menuTexts[menuSize - 3] = sf::Text(menuItems[menuSize - 3], font, 20);
        menuTexts[menuSize - 3].setFillColor(TEXT_COLOR);
        menuTexts[menuSize - 3].setPosition(sf::Vector2f(
            window->getSize().x,
            menuTexts[menuSize - 5].getPosition().y         // HOLD position y
        ));
        // Bar - Rectangle
        originalPositionX[menuSize - 3]   = window->getSize().x / 1.25 + menuBars[menuSize - 4].getSize().x;
        currentBarPositionX[menuSize - 3] = window->getSize().x;
        targetBarPositionX[menuSize - 3]  = originalPositionX[menuSize - 3];
        
        menuBars[menuSize - 3].setSize(menuBars[menuSize - 4].getSize());
        
        menuBars[menuSize - 3].setFillColor(MENU_BAR_COLOR);
        menuBars[menuSize - 3].setPosition(sf::Vector2f(
            currentBarPositionX[menuSize - 3] - BAR_PADDING,
            OPTION_PADDING + OPTION_PADDING * 2 - BAR_PADDING      
        ));
        // Disappear
        gradient[menuSize - 3] = sf::VertexArray(sf::Quads, 4);

        sf::Vector2f barPos  = menuBars[menuSize - 3].getPosition();
        sf::Vector2f barSize = menuBars[menuSize - 3].getSize();

        gradient[menuSize - 3][0].position = sf::Vector2f(barPos.x, barPos.y);
        gradient[menuSize - 3][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
        gradient[menuSize - 3][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
        gradient[menuSize - 3][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
        
        sf::Color startColor = MENU_BAR_COLOR;
        startColor.a = 255 / 2;
        sf::Color endColor   = MENU_BAR_COLOR;     
        endColor.a = 0; 

        gradient[menuSize - 3][0].color = startColor;
        gradient[menuSize - 3][3].color = startColor;
        gradient[menuSize - 3][1].color = endColor;
        gradient[menuSize - 3][2].color = endColor;
    }
}

Option::~Option() {
    delete soundManager;
    delete keyConfiguration;

    delete[] menuTexts;
    delete[] menuBars;

    delete[] originalPositionX;
    delete[] currentBarPositionX;
    delete[] targetBarPositionX;

    delete[] gradient;
}

bool Option::notSelected() {
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

STATUS_CODE Option::getSelectedItem() {
    selected = false;
    int cloneSelectedItem = selectedItem;
    selectedItem = 0;
    // switch(cloneSelectedItem) {
    //     case 0:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 1:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 2:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 3:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 4:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 5:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 6:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 7:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 8:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 9:  throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 10: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 11: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    //     case 12: return STATUS_CODE::BACK;
    //     default: throw std::invalid_argument("[Menu.cpp] - getSelectedItem() : STATUS_CODE (MENU_CODE::OPTION) error.");
    // }
    return STATUS_CODE::BACK;
}

void Option::processEvents(sf::RenderWindow *window, sf::Event event) {
    if (event.type == sf::Event::Closed) {
        if (optionSelected or audioSelected) {
            if (optionSelected == true) {
                std::string reWrite = menuTexts[optionSelectedIndex].getString();
                for (int i = 0; i < 10; i++) {
                    reWrite[i + 27] = optionKeyStrings[0][i];
                    reWrite[i + 40] = optionKeyStrings[1][i];
                }
                menuTexts[optionSelectedIndex].setString(reWrite);
            }
            
            optionSelected = false;
            audioSelected = false;

            soundManager->play("selected");
        }
        else {
            selectedItem = menuSize - 1;
            
            selected = true;

            soundManager->play("selected");

            selectedTimeout.restart();
        }
    }
    else if (selected) {
        // Do nothing
    }
    else if (optionSelected) {
        if (optionWaitForKey and optionWaitForReleaseKey and event.type == sf::Event::KeyPressed) {
            sf::Keyboard::Key optionNewKey = event.key.code;

            if (optionNewKey == sf::Keyboard::Escape) {
                std::string reWrite = menuTexts[optionSelectedIndex].getString();
                for (int i = 0; i < 10; i++) {
                    reWrite[i + 27] = optionKeyStrings[0][i];
                    reWrite[i + 40] = optionKeyStrings[1][i];
                }
                menuTexts[optionSelectedIndex].setString(reWrite);
            }
            else {
                keyConfiguration->removeKey(optionKeyStrings[optionSelectedItem]);

                if (optionNewKey != sf::Keyboard::Backspace) {
                    optionKeyStrings[optionSelectedItem] = keyConfiguration->getKeyName(optionNewKey);
                    while (optionKeyStrings[optionSelectedItem].size() != 10) {
                        optionKeyStrings[optionSelectedItem] += ' ';
                    }
                    keyConfiguration->setKey(static_cast<EVENT>(selectedItem - 1), optionNewKey);
                }

                keyConfiguration->saveKey("ASSETS/keybindings.txt");

                for (int i = 1; i < menuSize - 4; i++) {
                    std::pair<std::string, std::string> keys = keyConfiguration->getKey(static_cast<EVENT>(i - 1));
                    
                    while (keys.first .size() != 10) keys.first  += ' ';
                    while (keys.second.size() != 10) keys.second += ' ';

                    std::string newText = menuTexts[i].getString().substring(0, 24) + " [ " + keys.first + " | " + keys.second + " ]";
                    
                    menuTexts[i].setString(newText);
                }
            }

            optionSelected = false;            

            optionWaitForKey = false;
        }
        else if (event.type == sf::Event::KeyReleased) {
            optionWaitForReleaseKey = true;
        }
        else if (optionWaitForKey) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Lấy vị trí con trỏ trong cửa sổ
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
                    if (not optionKeyTexts[optionSelectedItem].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        optionSelected = false;
                        optionWaitForKey = false;
    
                        std::string reWrite = menuTexts[optionSelectedIndex].getString();
                        for (int i = 0; i < 10; i++) {
                            reWrite[i + 27] = optionKeyStrings[0][i];
                            reWrite[i + 40] = optionKeyStrings[1][i];
                        }
                        menuTexts[optionSelectedIndex].setString(reWrite);
                    }
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                optionSelected = false;
                
                std::string reWrite = menuTexts[optionSelectedIndex].getString();
                for (int i = 0; i < 10; i++) {
                    reWrite[i + 27] = optionKeyStrings[0][i];
                    reWrite[i + 40] = optionKeyStrings[1][i];
                }
                menuTexts[optionSelectedIndex].setString(reWrite);

                soundManager->play("selected");
            }
            else if (event.key.code == sf::Keyboard::Backspace) {
                keyConfiguration->removeKey(optionKeyStrings[optionSelectedItem]);

                optionSelected = false;
                
                keyConfiguration->saveKey("ASSETS/keybindings.txt");

                for (int i = 1; i < menuSize - 4; i++) {
                    std::pair<std::string, std::string> keys = keyConfiguration->getKey(static_cast<EVENT>(i - 1));
                    
                    while (keys.first .size() != 10) keys.first  += ' ';
                    while (keys.second.size() != 10) keys.second += ' ';

                    std::string newText = menuTexts[i].getString().substring(0, 24) + " [ " + keys.first + " | " + keys.second + " ]";
                    
                    menuTexts[i].setString(newText);
                }

                soundManager->play("selected");
            }
            else if (event.key.code == sf::Keyboard::Left or event.key.code == sf::Keyboard::A) {
                optionSelectedItem ^= 1;
                
                mouseSelect = false;

                soundManager->play("move");
            }
            else if (event.key.code == sf::Keyboard::Right or event.key.code == sf::Keyboard::D) {
                optionSelectedItem ^= 1;
                
                mouseSelect = false;

                soundManager->play("move");
            }
            else if (event.key.code == sf::Keyboard::Space or event.key.code == sf::Keyboard::Enter) {
                optionWaitForKey = true;
                optionWaitForReleaseKey = true;

                soundManager->play("selected");
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Lấy vị trí con trỏ trong cửa sổ
                bool quitEdit = true;
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
                for (int i = 0; i <= 1; i++) {
                    if (optionKeyTexts[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        optionSelectedItem = i;
                        
                        optionWaitForKey = true;

                        soundManager->play("selected");

                        quitEdit = false;

                        break;
                    }
                }
                if (quitEdit) {
                    optionSelected = false;

                    std::string reWrite = menuTexts[optionSelectedIndex].getString();
                    for (int i = 0; i < 10; i++) {
                        reWrite[i + 27] = optionKeyStrings[0][i];
                        reWrite[i + 40] = optionKeyStrings[1][i];
                    }
                    menuTexts[optionSelectedIndex].setString(reWrite);
                }
            }
        }
    } 
    else if (audioSelected) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up or event.key.code == sf::Keyboard::W) {
                if (selectedItem == menuSize - 4) {
                    soundManager->increaseVolume();
                }
                else {
                    soundManager->increaseMusicVolume();
                }
    
                soundManager->play("scroll");
            } 
            else if (event.key.code == sf::Keyboard::Down or event.key.code == sf::Keyboard::S) {
                if (selectedItem == menuSize - 4) {
                    soundManager->decreaseVolume();
                }
                else {
                    soundManager->decreaseMusicVolume();
                }
    
                soundManager->play("scroll");
            } 
            else if (event.key.code == sf::Keyboard::Space or event.key.code == sf::Keyboard::Enter or event.key.code == sf::Keyboard::Escape) {
                audioSelected = false;

                soundManager->play("selected");
            }
        }
        else if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                if (event.mouseWheelScroll.delta > 0) {  // Scroll up
                    if (selectedItem == menuSize - 4) {
                        soundManager->increaseVolume();
                    }
                    else {
                        soundManager->increaseMusicVolume();
                    }
                } else {  // Scroll down
                    if (selectedItem == menuSize - 4) {
                        soundManager->decreaseVolume();
                    }
                    else {
                        soundManager->decreaseMusicVolume();
                    }
                }
                
                soundManager->play("scroll");
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Lấy vị trí con trỏ trong cửa sổ
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
                if (not menuBars[selectedItem].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    audioSelected = false;

                    soundManager->play("selected");
                }
            }
        }
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
        else if (event.key.code == sf::Keyboard::Escape) {  // Back
            selectedItem = menuSize - 1;
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
    else if (selectedItem == menuSize - 4 or selectedItem == menuSize - 3) {
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                if (event.mouseWheelScroll.delta > 0) {  // Scroll up
                    if (selectedItem == menuSize - 4) {
                        soundManager->increaseVolume();
                    }
                    else {
                        soundManager->increaseMusicVolume();
                    }
                } else {  // Scroll down
                    if (selectedItem == menuSize - 4) {
                        soundManager->decreaseVolume();
                    }
                    else {
                        soundManager->decreaseMusicVolume();
                    }
                }
                
                soundManager->play("scroll");
            }
        }
    }
}

void Option::update(sf::RenderWindow *window) {
    // Lấy vị trí con trỏ trong cửa sổ
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window); 
    if (mousePos != mousePosPrev) { mouseSelect = true; }
    mousePosPrev = mousePos;

    if (optionSelected) {       // Option - Control
        if (mouseSelect and not optionWaitForKey) {
            for (int i = 0; i <= 1; i++) {
                if (optionKeyTexts[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    if (optionSelectedItem != i) {
                        optionSelectedItem = i;
                        
                        soundManager->play("move");
                    }
                }
            }
        }
        else if (optionWaitForKey) {
            if (optionSelectedItem == 0) {
                optionKeyTexts[0].setString("  input...  ");
            }
            else {
                optionKeyTexts[1].setString("  input...  ");
            }
        }
    }
    else if (audioSelected) {
        // disable mouse selected
    }
    else {                      // Normal
        if (not selected and mouseSelect) {
            for (int i = 0; i < menuSize; i++) {
                if (menuBars[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    if (selectedItem != i) {
                        selectedItem = i;
                        
                        if (menuTexts[i].getFillColor() == TEXT_COLOR) {
                            soundManager->play("move");
                        }
                    }
                }
            }
        }
    }
    
    if (selected and selectedItem != menuSize - 1) {
        // Control
        if (1 <= selectedItem and selectedItem <= menuSize - 5) {
            optionSelected = true;
            optionSelectedIndex = selectedItem;
            optionSelectedItem = 0;

            //                            [27]         [40]
            //                             |            |
            //                             v            v
            // "ROTATE COUNTERCLOCKWISE  [ ---------- | ---------- ]"
            optionKeyStrings[0] = menuTexts[optionSelectedIndex].getString().substring(27, 10);
            optionKeyStrings[1] = menuTexts[optionSelectedIndex].getString().substring(40, 10);
            menuTexts[optionSelectedIndex].setString(menuTexts[optionSelectedIndex].getString().substring(0, 25) + "[            |            ]");

            sf::Text temporary("A", font, 20);
            optionKeyTexts[0] = sf::Text(' ' + (optionKeyStrings[0][0] != ' ' ? optionKeyStrings[0] : " (create) ") + ' ', font, 20);
            optionKeyTexts[0].setPosition(sf::Vector2f(
                targetBarPositionX[optionSelectedIndex] + temporary.getGlobalBounds().width * 26,
                menuTexts[optionSelectedIndex].getPosition().y
            ));

            optionKeyTexts[1] = sf::Text(' ' + (optionKeyStrings[1][0] != ' ' ? optionKeyStrings[1] : " (create) ") + ' ', font, 20);
            optionKeyTexts[1].setPosition(sf::Vector2f(
                targetBarPositionX[optionSelectedIndex] + temporary.getGlobalBounds().width * 39,
                menuTexts[optionSelectedIndex].getPosition().y
            ));
        }
        // Audio
        if (menuSize - 4 <= selectedItem and selectedItem <= menuSize - 3) {
            audioSelected = true;
        }

        selected = false;
    }

    if (optionSelected) {
        for (int i = 0; i <= 1; i++) {
            std::string temporary = optionKeyTexts[i].getString();
            if (optionSelectedItem == i) {
                optionKeyTexts[i].setFillColor(sf::Color::Red);
                temporary[0] = '>';
                temporary.back() = '<';
            }
            else {
                optionKeyTexts[i].setFillColor(SELECTED_COLOR);
                temporary[0] = ' ';
                temporary.back() = ' ';
            }
            optionKeyTexts[i].setString(temporary);
        }
    }

    for (int i = 0; i < menuSize; i++) {
        if (not selected) {
            menuTexts[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
            targetBarPositionX[i] = originalPositionX[i];
        }
        else {
            menuTexts[i].setFillColor(i == selectedItem ? SELECTED_COLOR : TEXT_COLOR);
            targetBarPositionX[i] = window->getSize().x + SELECTED_PADDING;
        }

        currentBarPositionX[i] += (targetBarPositionX[i] - currentBarPositionX[i]) * SLIDE_SPEED;

        menuTexts[i].setPosition(sf::Vector2f(currentBarPositionX[i], menuTexts[i].getPosition().y));
        menuBars[i].setPosition(sf::Vector2f(currentBarPositionX[i] - BAR_PADDING, menuBars[i].getPosition().y));

        if (i >= menuSize - 4) {
            sf::Vector2f barPos  = menuBars[i].getPosition();
            sf::Vector2f barSize = menuBars[i].getSize();

            gradient[i][0].position = sf::Vector2f(barPos.x, barPos.y);
            gradient[i][1].position = sf::Vector2f(barPos.x + barSize.x, barPos.y);
            gradient[i][2].position = sf::Vector2f(barPos.x + barSize.x, barPos.y + barSize.y);
            gradient[i][3].position = sf::Vector2f(barPos.x, barPos.y + barSize.y);
        }
    }
    // Controls
    if (1 <= selectedItem and selectedItem <= menuSize - 5) {
        menuTexts[0].setFillColor(SELECTED_COLOR);
    }
    // Audio
    else if (menuSize - 4 <= selectedItem and selectedItem <= menuSize - 3) {
        menuTexts[menuSize - 2].setFillColor(SELECTED_COLOR);

        if (audioSelected) {
            menuTexts[selectedItem].setFillColor(sf::Color::Red);
        }
    }
}

void Option::draw(sf::RenderWindow *window) {
    window->draw(menuTitleBar);
    window->draw(menuTitle);

    window->draw(footerBar);
    window->draw(footerVersion);
    window->draw(footerOwner);

    // Controls
    window->draw(menuBars[0]);
    window->draw(menuTexts[0]);
    
    // Key - Controls
    for (int i = 1; i < menuSize - 4; i++) {
        window->draw(menuBars[i]);
        window->draw(menuTexts[i]);
    }

    if (optionSelected) {
        window->draw(optionKeyTexts[0]);
        window->draw(optionKeyTexts[1]);

        sf::Text help("Exit [Esc], Delete [Backspace]", font, 20);
        help.setPosition(
            menuBars[0].getPosition().x + menuBars[0].getGlobalBounds().width  - help.getGlobalBounds().width - BAR_PADDING,
            menuBars[0].getPosition().y + menuBars[0].getGlobalBounds().height - help.getGlobalBounds().height * 1.5
        );
        window->draw(help);
    }

    // Audio
    window->draw(gradient[menuSize - 2]);
    window->draw(menuTexts[menuSize - 2]);

    // SFX
    window->draw(gradient[menuSize - 4]);
    menuTexts[menuSize - 4].setPosition(
        // menuTexts[menuSize - 4].getPosition().x,
        menuBars[menuSize - 4].getPosition().x + menuBars[menuSize - 4].getSize().x / 2 - menuTexts[menuSize - 4].getLocalBounds().width / 2 - 1.5,
        menuTexts[menuSize - 4].getPosition().y
    );
    window->draw(menuTexts[menuSize - 4]);
    
    float sfxMaxHeight = (menuTexts[menuSize - 4].getPosition().y - menuBars[menuSize - 4].getPosition().y) - menuTexts[menuSize - 4].getGlobalBounds().height + BAR_PADDING;
    float sfxHeight    = sfxMaxHeight * soundManager->getVolume() / 100;
    sf::RectangleShape sfxBar(sf::Vector2f(
        BAR_PADDING * 1.5, 
        sfxHeight
    ));
    sfxBar.setPosition(
        menuBars[menuSize - 4].getPosition().x + menuBars[menuSize - 4].getSize().x / 2 - sfxBar.getSize().x / 2,
        menuBars[menuSize - 4].getPosition().y + sfxMaxHeight - sfxHeight - BAR_PADDING
    );
    window->draw(sfxBar);

    // Music
    window->draw(gradient[menuSize - 3]);
    menuTexts[menuSize - 3].setPosition(
        // menuTexts[menuSize - 3].getPosition().x,
        menuBars[menuSize - 3].getPosition().x + menuBars[menuSize - 3].getSize().x / 2 - menuTexts[menuSize - 3].getGlobalBounds().width / 2 - 1.5,
        menuTexts[menuSize - 3].getPosition().y
    );
    window->draw(menuTexts[menuSize - 3]);
    
    float musicMaxHeight = (menuTexts[menuSize - 3].getPosition().y - menuBars[menuSize - 3].getPosition().y) - menuTexts[menuSize - 3].getGlobalBounds().height + BAR_PADDING;
    float musicHeight    = musicMaxHeight * soundManager->getMusicVolume() / 100;
    sf::RectangleShape musicBar(sf::Vector2f(
        BAR_PADDING * 1.5, 
        musicHeight
    ));
    musicBar.setPosition(
        menuBars[menuSize - 3].getPosition().x + menuBars[menuSize - 3].getSize().x / 2 - musicBar.getSize().x / 2,
        menuBars[menuSize - 3].getPosition().y + musicMaxHeight - musicHeight - BAR_PADDING
    );
    window->draw(musicBar);


    // Back
    window->draw(gradient[menuSize - 1]);
    window->draw(menuTexts[menuSize - 1]);
}