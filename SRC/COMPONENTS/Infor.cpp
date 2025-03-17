#include "Infor.hpp"

#include "SoundManager.hpp"

const int FONT_SIZE = 25;

constexpr float TIME_OUT = 2.0f;
const std::string clearMessage[5] = { std::string(), "SINGLE", "DOUBLE", "TRIPLE", "QUAD" };

Infor::Infor(int x, int y, int w) : INFOR_POSITION_X(x), INFOR_POSITION_Y(y), INFOR_WIDTH(w), nLine(0), count(0), add(0), B2B(0), B2BMissing(0) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
    
    soundManager = new SoundManager();
    soundManager->loadSound("clearB2B"  , "ASSETS/sfx/clearbtb.mp3");
    soundManager->loadSound("clearLine" , "ASSETS/sfx/clearline.mp3");
    soundManager->loadSound("clearQuad" , "ASSETS/sfx/clearquad.mp3");
    soundManager->loadSound("clearSpin" , "ASSETS/sfx/clearspin.mp3");
    soundManager->loadSound("combo1"    , "ASSETS/sfx/combo/combo_1.mp3");
    soundManager->loadSound("combo2"    , "ASSETS/sfx/combo/combo_2.mp3");
    soundManager->loadSound("combo3"    , "ASSETS/sfx/combo/combo_3.mp3");
    soundManager->loadSound("combo4"    , "ASSETS/sfx/combo/combo_4.mp3");
    soundManager->loadSound("combo5"    , "ASSETS/sfx/combo/combo_5.mp3");
    soundManager->loadSound("comboBreak", "ASSETS/sfx/combo/combobreak.mp3");
}

Infor::~Infor() {
    delete soundManager;
}

void Infor::reset() {
    nLine = 0; count = 0; add = 0; B2B = 0; B2BMissing = 0;
}

void Infor::addLine(uint8_t lines, bool spin) {
    nLine   += lines;
    add      = lines;

    if (add != 0) {
        count++;

        message = clearMessage[add];
        
        if (count > 1) {
            combo = std::to_string(count - 1);
            
            soundManager->play("combo" + (count <= 6 ? combo : "5"));

            comboTimeout.restart();
        } 
        
        timeout.restart();

        if (add == 4) {
            B2B++;

            if (B2B > 1) {
                soundManager->play("clearB2B");
            }
            else {
                soundManager->play("clearQuad");
            }
        }
        else if (spin) {
            B2B++;

            soundManager->play("clearSpin");
        }
        else {
            if (B2B > 1) {
                B2BMissing = B2B;

                B2BMissingTimeout.restart();
            }

            B2B = 0;

            soundManager->play("clearLine");
        }
    } else {
        if (count > 2) {
            soundManager->play("comboBreak");
        }

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
    
    float alpha = 255 * (1 - comboTimeout.getElapsedTime().asSeconds() / TIME_OUT);
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
    float phase = B2BMissingTimeout.getElapsedTime().asSeconds() / TIME_OUT;
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
    }
    
    if (comboTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        if (combo != std::string()) {
            drawCombo(window, combo);
        }
    } 
    else {
        combo.clear();
    }

    if (B2BMissingTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        if (B2B <= 1 and B2BMissing > 1) {
            drawMissingB2B(window);
        }
    } 
    else {
        B2BMissing = 0;
    }

    if (B2B > 1) {
        drawB2B(window);
    }
}