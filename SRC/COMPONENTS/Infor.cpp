#include "Infor.hpp"

const int FONT_SIZE = 25;

constexpr float TIME_OUT = 2.0f;
const std::string clearMessage[5] = { std::string(), "SINGLE", "DOUBLE", "TRIPLE", "QUAD" };

Infor::Infor(int x, int y, int w) : INFOR_POSITION_X(x), INFOR_POSITION_Y(y), INFOR_WIDTH(w), nLine(0), count(0), add(0), B2B(0), B2BMissing(0) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
}

void Infor::addLine(int lines) {
    nLine   += lines;
    add      = lines;

    if (add != 0) {
        count++;

        message = clearMessage[add];
        
        if (count > 1) {
            combo = std::to_string(count - 1);
        } 
        else {
            combo = std::string();
        }
        
        timeout.restart();

        if (add == 4) {
            B2B++;
        }
        else {
            if (B2B > 1) {
                B2BMissing = B2B;
            }

            B2B = 0;
        }
    } else {
        count = 0;
    }
}

void Infor::drawMessage(sf::RenderWindow *window, const std::string string) {
    sf::Text text(string, font, FONT_SIZE);
    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y);
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void Infor::drawCombo(sf::RenderWindow *window, const std::string string) {
    sf::Text text("COMBO", font, FONT_SIZE);
    sf::Text number(string, font, FONT_SIZE * 2);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE * 2);
    number.setPosition(text.getPosition().x - number.getGlobalBounds().width - 15, INFOR_POSITION_Y + FONT_SIZE + FONT_SIZE / 2);
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));
    number.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(number);
    window->draw(text);
}

void Infor::drawB2B(sf::RenderWindow *window) {
    sf::Text text("B2B x" + std::to_string(B2B - 1), font, FONT_SIZE - FONT_SIZE / 4);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE * 1);
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, (alpha > 175 ? 255 : 215), 0, std::max(alpha, 175.0f)));

    window->draw(text);
}

void Infor::drawMissingB2B(sf::RenderWindow *window) {
    sf::Text text("B2B x" + std::to_string(B2BMissing - 1), font, FONT_SIZE - FONT_SIZE / 4);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE * 1);
    
    sf::Color color;
    float phase = timeout.getElapsedTime().asSeconds() / TIME_OUT;
    if (phase < 0.075f) {     
        color = sf::Color::White;
    } 
    else if (phase < 0.15f) {
        color = sf::Color::Black;
    } 
    else if (phase < 0.225f) {     
        color = sf::Color::White;
    } 
    else if (phase < 0.3f) {
        color = sf::Color::Black;
    } 
    else if (phase < 0.375f) { 
        color = sf::Color(255, 215, 0);
    } 

    text.setFillColor(color);

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

        if (B2B <= 1 and B2BMissing > 1) {
            drawMissingB2B(window);
        }
    } 
    else {
        combo.clear();

        B2BMissing = 0;
    }

    if (B2B > 1) {
        drawB2B(window);
    }
}