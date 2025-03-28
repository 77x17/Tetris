#include "Infor.hpp"

#include "SoundManager.hpp"
#include "Common.hpp"

const int FONT_SIZE = 25;

const std::string clearMessage[5] = { std::string(), "SINGLE", "DOUBLE", "TRIPLE", "QUAD" };

Infor::Infor(): nLine(0), count(0), B2B(0), B2BMissing(0), spin(false), spinDraw(false) {
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

void Infor::setPosition(int x, int y, int w) {
    INFOR_POSITION_X = x; INFOR_POSITION_Y = y; INFOR_WIDTH = w; 
}

void Infor::setTimer() {
    runningTime.restart();
}

Infor::~Infor() {
    delete soundManager;
}

void Infor::reset() { nLine = 0; count = 0; B2B = 0; B2BMissing = 0;}

// garbage sent
uint8_t Infor::removeLine(uint8_t lines) {
    if (lines == 0) {
        // nothing
        return 0;
    }
    nLine += lines;
    return nLine;
}

void Infor::update(uint8_t lines, bool isSpin, char block) {
    if (isSpin) {
        spin      = true;
        spinDraw  = true;
        typeBlock = block;

        spinTimeout.restart();
    }
    else {
        spin = false;
    }

    if (lines == 0) {
        return;
    }

    if (lines != 0) {
        count++;

        message = clearMessage[lines];
        
        if (count > 1) {
            combo = std::to_string(count - 1);

            comboTimeout.restart();
        }
        
        timeout.restart();

        if (lines == 4) {
            B2B++;
        }
        else if (isSpin) {
            B2B++;
        }
        else {
            if (B2B > 1) {
                B2BMissing = B2B;

                B2BMissingTimeout.restart();
            }

            B2B = 0;
        }
    }
}

void Infor::playSound(uint8_t lines, bool isSpin, char block) {
    if (lines == 0) {
        if (count > 2) {
            soundManager->play("comboBreak");
        }

        count = 0;
        return;
    }

    if (lines != 0) {
        if (count > 1) {
            soundManager->play("combo" + (count <= 6 ? combo : "5"));
        }
        
        if (lines == 4) {
            if (B2B > 1) {
                soundManager->play("clearB2B");
            }
            else {
                soundManager->play("clearQuad");
            }
        }
        else if (isSpin) {
            soundManager->play("clearSpin");
        }
        else {
            soundManager->play("clearLine");
        }
    }
}

void Infor::drawMessage(sf::RenderWindow *window, const std::string string) {
    sf::Text text(string, font, FONT_SIZE);
    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE);
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void Infor::drawCombo(sf::RenderWindow *window, const std::string string) {
    sf::Text text("COMBO", font, FONT_SIZE);
    sf::Text number(string, font, FONT_SIZE * 2);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE * 3);
    number.setPosition(text.getPosition().x - number.getGlobalBounds().width - 15, INFOR_POSITION_Y + FONT_SIZE * 2 + FONT_SIZE / 2);
    
    float alpha = 255 * (1 - comboTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));
    number.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(number);
    window->draw(text);
}

void Infor::drawB2B(sf::RenderWindow *window) {
    sf::Text text("B2B x" + std::to_string(B2B - 1), font, FONT_SIZE - FONT_SIZE / 4);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE * 2);
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, (alpha > 175 ? 255 : 215), 0, std::max(alpha, 175.0f)));

    window->draw(text);
}

void Infor::drawMissingB2B(sf::RenderWindow *window) {
    sf::Text text("B2B x" + std::to_string(B2BMissing - 1), font, FONT_SIZE - FONT_SIZE / 4);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y + FONT_SIZE * 2);
    
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
        color = sf::Color::White;
    } 
    else {
        return;
    }

    text.setFillColor(color);

    window->draw(text);
}

void Infor::drawSpin(sf::RenderWindow *window) {
    std::string type = std::string() + typeBlock;
    sf::Text text(type + " - SPIN", font, FONT_SIZE - FONT_SIZE / 4);

    text.setPosition(INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, INFOR_POSITION_Y);
    
    float alpha = 255 * (1 - spinTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    sf::Color color;
    switch (typeBlock) {
        case 'I':
            color = sf::Color(103, 248, 254, alpha);
            break;
        case 'L':
            color = sf::Color(254, 183, 103, alpha);
            break;
        case 'J':
            color = sf::Color(103, 105, 254, alpha);
            break;
        case 'O':
            color = sf::Color(254, 247, 103, alpha);
            break;
        case 'Z':
            color = sf::Color(254, 103, 103, alpha);
            break;
        case 'S':
            color = sf::Color(123, 255, 104, alpha);
            break;
        case 'T':
            color = sf::Color(191, 64, 191, alpha);
            break;
    }
    text.setFillColor(color);

    window->draw(text);
}

void Infor::draw(sf::RenderWindow *window) {
    if (message != std::string() and timeout.getElapsedTime().asSeconds() < TIME_OUT) {
        drawMessage(window, message);
    }

    if (spinDraw and spinTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        drawSpin(window);
    }
    else {
        spinDraw = false;
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