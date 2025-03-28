#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <string>
#include <unordered_map>

enum EVENT {
    MOVE_LEFT               = 0,
    MOVE_RIGHT              = 1,
    MOVE_DOWN               = 2,
    HARD_DROP               = 3,
    ROTATE_CLOCKWISE        = 4,
    ROTATE_COUNTERCLOCKWISE = 5,
    ROTATE_180              = 6,
    HOLD_PIECE              = 7,
    
    NOTHING                 = -1
};

class KeyConfiguration {
private:
    std::unordered_map<sf::Keyboard::Key, EVENT> keyBindings;

public:
    KeyConfiguration();
    KeyConfiguration(std::string filename);

    EVENT getEvent(const sf::Keyboard::Key &key) const;
    
    std::pair<std::string, std::string> getKey(EVENT event) const;
    void removeKey(std::string key);
    void setKey(EVENT event, sf::Keyboard::Key key);
    std::string getKeyName(sf::Keyboard::Key key);
    void saveKey(std::string filename);
};