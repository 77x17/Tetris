#include "Infor.hpp"

const int FONT_SIZE = 25;

constexpr float TIME_OUT = 1.5f;
const std::string clearMessage[5] = { std::string(), "SINGLE", "DOUBLE", "TRIPLE", "QUAD" };

Infor::Infor(int x, int y) : INFOR_POSITION_X(x), INFOR_POSITION_Y(y), nLine(0), count(0), add(0), drawAble(false) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

void Infor::addLine(int lines) {
    nLine   += lines;
    add      = lines;
    drawAble = false;

    if (add != 0) {
        count++;

        message = clearMessage[add];
        
        if (count > 1) {
            combo = std::to_string(count - 1);
        }
        timeout.restart();
    } else {
        count = 0;
    }
}

void Infor::drawMessage(sf::RenderWindow *window, const std::string string) {
    sf::Text text(string, font, FONT_SIZE);
    text.setPosition(INFOR_POSITION_X, INFOR_POSITION_Y);
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void Infor::drawCombo(sf::RenderWindow *window, const std::string string) {
    sf::Text text("COMBO", font, FONT_SIZE);
    sf::Text number(string, font, FONT_SIZE * 2);

    sf::FloatRect textBounds = text.getGlobalBounds();
    sf::FloatRect numberBounds = number.getGlobalBounds();

    text.setPosition(INFOR_POSITION_X, INFOR_POSITION_Y + FONT_SIZE * 2);
    number.setPosition(text.getPosition().x - numberBounds.width - 10, // Dịch sang trái, tạo khoảng cách 10px
                       text.getPosition().y + textBounds.height / 2 - numberBounds.height / 2);

    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));
    number.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(number);
    window->draw(text);
}

void Infor::draw(sf::RenderWindow *window) {
    if (timeout.getElapsedTime().asSeconds() < TIME_OUT) {
        if (message != std::string()) {
            drawMessage(window, message);
        }

        if (combo != std::string()) {
            drawCombo(window, combo);
        }
    } 
    else {
        combo.clear();
    }
}