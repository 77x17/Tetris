#pragma once

#include <SFML/Graphics.hpp>

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