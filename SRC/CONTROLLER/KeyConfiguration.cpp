#include "KeyConfiguration.hpp"

#include <unordered_map>

#include <fstream>

// Mảng ánh xạ sf::Keyboard::Key sang chuỗi
const std::string keyNames[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9",
    "Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt", "RSystem",
    "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period", "Quote", "Slash",
    "Backslash", "Tilde", "Equal", "Dash", "Space", "Return", "Backspace", "Tab",
    "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract",
    "Multiply", "Divide", "Left", "Right", "Up", "Down",
    "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15",
    "Pause"
};

std::unordered_map<std::string, sf::Keyboard::Key> keyMap = {
    {"A", sf::Keyboard::A}, {"B", sf::Keyboard::B}, {"C", sf::Keyboard::C},
    {"D", sf::Keyboard::D}, {"E", sf::Keyboard::E}, {"F", sf::Keyboard::F},
    {"G", sf::Keyboard::G}, {"H", sf::Keyboard::H}, {"I", sf::Keyboard::I},
    {"J", sf::Keyboard::J}, {"K", sf::Keyboard::K}, {"L", sf::Keyboard::L},
    {"M", sf::Keyboard::M}, {"N", sf::Keyboard::N}, {"O", sf::Keyboard::O},
    {"P", sf::Keyboard::P}, {"Q", sf::Keyboard::Q}, {"R", sf::Keyboard::R},
    {"S", sf::Keyboard::S}, {"T", sf::Keyboard::T}, {"U", sf::Keyboard::U},
    {"V", sf::Keyboard::V}, {"W", sf::Keyboard::W}, {"X", sf::Keyboard::X},
    {"Y", sf::Keyboard::Y}, {"Z", sf::Keyboard::Z}, {"Num0", sf::Keyboard::Num0},
    {"Num1", sf::Keyboard::Num1}, {"Num2", sf::Keyboard::Num2}, {"Num3", sf::Keyboard::Num3},
    {"Num4", sf::Keyboard::Num4}, {"Num5", sf::Keyboard::Num5}, {"Num6", sf::Keyboard::Num6},
    {"Num7", sf::Keyboard::Num7}, {"Num8", sf::Keyboard::Num8}, {"Num9", sf::Keyboard::Num9},
    {"Escape", sf::Keyboard::Escape}, {"LControl", sf::Keyboard::LControl},
    {"LShift", sf::Keyboard::LShift}, {"LAlt", sf::Keyboard::LAlt},
    {"LSystem", sf::Keyboard::LSystem}, {"RControl", sf::Keyboard::RControl},
    {"RShift", sf::Keyboard::RShift}, {"RAlt", sf::Keyboard::RAlt},
    {"RSystem", sf::Keyboard::RSystem}, {"Menu", sf::Keyboard::Menu},
    {"LBracket", sf::Keyboard::LBracket}, {"RBracket", sf::Keyboard::RBracket},
    {"Semicolon", sf::Keyboard::Semicolon}, {"Comma", sf::Keyboard::Comma},
    {"Period", sf::Keyboard::Period}, {"Quote", sf::Keyboard::Quote},
    {"Slash", sf::Keyboard::Slash}, {"Backslash", sf::Keyboard::Backslash},
    {"Tilde", sf::Keyboard::Tilde}, {"Equal", sf::Keyboard::Equal},
    {"Dash", sf::Keyboard::Dash}, {"Space", sf::Keyboard::Space},
    {"Return", sf::Keyboard::Enter}, {"Backspace", sf::Keyboard::Backspace},
    {"Tab", sf::Keyboard::Tab}, {"PageUp", sf::Keyboard::PageUp},
    {"PageDown", sf::Keyboard::PageDown}, {"End", sf::Keyboard::End},
    {"Home", sf::Keyboard::Home}, {"Insert", sf::Keyboard::Insert},
    {"Delete", sf::Keyboard::Delete}, {"Add", sf::Keyboard::Add},
    {"Subtract", sf::Keyboard::Subtract}, {"Multiply", sf::Keyboard::Multiply},
    {"Divide", sf::Keyboard::Divide}, {"Left", sf::Keyboard::Left},
    {"Right", sf::Keyboard::Right}, {"Up", sf::Keyboard::Up},
    {"Down", sf::Keyboard::Down}, {"Numpad0", sf::Keyboard::Numpad0},
    {"Numpad1", sf::Keyboard::Numpad1}, {"Numpad2", sf::Keyboard::Numpad2},
    {"Numpad3", sf::Keyboard::Numpad3}, {"Numpad4", sf::Keyboard::Numpad4},
    {"Numpad5", sf::Keyboard::Numpad5}, {"Numpad6", sf::Keyboard::Numpad6},
    {"Numpad7", sf::Keyboard::Numpad7}, {"Numpad8", sf::Keyboard::Numpad8},
    {"Numpad9", sf::Keyboard::Numpad9}, {"F1", sf::Keyboard::F1},
    {"F2", sf::Keyboard::F2}, {"F3", sf::Keyboard::F3}, {"F4", sf::Keyboard::F4},
    {"F5", sf::Keyboard::F5}, {"F6", sf::Keyboard::F6}, {"F7", sf::Keyboard::F7},
    {"F8", sf::Keyboard::F8}, {"F9", sf::Keyboard::F9}, {"F10", sf::Keyboard::F10},
    {"F11", sf::Keyboard::F11}, {"F12", sf::Keyboard::F12}, {"F13", sf::Keyboard::F13},
    {"F14", sf::Keyboard::F14}, {"F15", sf::Keyboard::F15}, {"Pause", sf::Keyboard::Pause}
};

// Default keyBindings
KeyConfiguration::KeyConfiguration() {
    keyBindings[sf::Keyboard::Left]  = MOVE_LEFT;
    keyBindings[sf::Keyboard::A]     = MOVE_LEFT;
    keyBindings[sf::Keyboard::Right] = MOVE_RIGHT;
    keyBindings[sf::Keyboard::D]     = MOVE_RIGHT;
    keyBindings[sf::Keyboard::Down]  = MOVE_DOWN;
    keyBindings[sf::Keyboard::S]     = MOVE_DOWN;
    keyBindings[sf::Keyboard::Space] = HARD_DROP;
    keyBindings[sf::Keyboard::Space] = HARD_DROP;
    keyBindings[sf::Keyboard::Up]    = ROTATE_CLOCKWISE;
    keyBindings[sf::Keyboard::X]     = ROTATE_CLOCKWISE;
    keyBindings[sf::Keyboard::Z]     = ROTATE_COUNTERCLOCKWISE;
    keyBindings[sf::Keyboard::Z]     = ROTATE_COUNTERCLOCKWISE;
    keyBindings[sf::Keyboard::A]     = ROTATE_180;
    keyBindings[sf::Keyboard::A]     = ROTATE_180;
    keyBindings[sf::Keyboard::C]     = HOLD_PIECE;
    keyBindings[sf::Keyboard::C]     = HOLD_PIECE;
}

KeyConfiguration::KeyConfiguration(std::string filename) {
    std::ifstream input(filename);
    if (not input.is_open()) {
        throw std::invalid_argument("[KeyConfiguration.cpp] - KeyConfiguration(): Invalid filename.");
    }

    for (int i = 0; i <= 7; i++) {
        int keyValue;

        input >> keyValue;
        if (keyValue != -1) {
            keyBindings[static_cast<sf::Keyboard::Key>(keyValue)] = static_cast<EVENT>(i);
        }
        input >> keyValue;
        if (keyValue != -1) {
            keyBindings[static_cast<sf::Keyboard::Key>(keyValue)] = static_cast<EVENT>(i);
        }
    }   

    input.close();
}

EVENT KeyConfiguration::getEvent(const sf::Keyboard::Key &key) const {
    auto iterator = keyBindings.find(key);
    if (iterator == keyBindings.end()) {
        return NOTHING;
    }

    return iterator->second;
}

std::pair<std::string, std::string> KeyConfiguration::getKey(EVENT event) const {
    std::pair<std::string, std::string> result = { "", "" };

    for (const auto &iterator : keyBindings) {
        if (iterator.second == event) {
            if (result.first == "") {
                result.first = keyNames[iterator.first];
            }
            else {
                result.second = keyNames[iterator.first];
            }
        }
    }

    if (result.first > result.second) {
        std::swap(result.first, result.second);
    }

    return result;
}

void KeyConfiguration::removeKey(std::string key) {
    while (not key.empty() and key.back() == ' ') {
        key.pop_back();
    }

    if (key.empty()) {
        return;
    }

    keyBindings.erase(keyMap[key]);
}


void KeyConfiguration::setKey(EVENT event, sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Unknown) {
        throw std::invalid_argument("[KeyConfiguration.cpp] - setKey(): key error.");
    }

    keyBindings[key] = event;
}

std::string KeyConfiguration::getKeyName(sf::Keyboard::Key key) {
    return keyNames[key];
}

void KeyConfiguration::saveKey(std::string filename) {
    std::ofstream output(filename);

    for (int i = 0; i <= 7; i++) {
        int count = 0;
        for (const auto &iterator : keyBindings) {
            if (iterator.second == static_cast<EVENT>(i)) {
                count++;
                output << iterator.first << '\n';
            }
        }
        while (count != 2) {
            output << -1 << '\n';
            ++count;
        }
    }

    output.close();
}