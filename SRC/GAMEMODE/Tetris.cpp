#include "Tetris.hpp"

#include "Menu.hpp"
#include "Scene.hpp"

Tetris::Tetris(sf::RenderWindow* win, Scene* s): window(win), scene(s){}

Tetris::~Tetris(){}

void Tetris::loadPlayground(sf::Texture &backgroundTexture, sf::Sprite &backgroundSprite) {
    backgroundTexture.loadFromFile("ASSETS/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 50));

    // Get window size & texture size
    sf::Vector2u windowSize  = window->getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    // Calculate scale factors
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    float scale = std::max(scaleX, scaleY);
    // Apply scale to fit window
    backgroundSprite.setScale(scale, scale);
    // Center the sprite
    float newWidth  = textureSize.x * scale;
    float newHeight = textureSize.y * scale;
    float posX = (windowSize.x - newWidth ) / 2;
    float posY = (windowSize.y - newHeight) / 2;
    backgroundSprite.setPosition(posX, posY);
}

