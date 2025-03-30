#include "Infor.hpp"

#include "SoundManager.hpp"
#include "Common.hpp"

#include <iostream>
#include <cstdio>

const int   FONT_SIZE = BLOCK_SIZE;

int MESSAGE_FONT_SIZE = FONT_SIZE;
int   MESSAGE_PADDING = FONT_SIZE;

int   COMBO_FONT_SIZE = FONT_SIZE * 2;
int     COMBO_PADDING = FONT_SIZE * 3;
int   N_COMBO_PADDING = FONT_SIZE * 2 + FONT_SIZE / 2;

int     B2B_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
int       B2B_PADDING = FONT_SIZE * 2;

int    SPIN_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;

int     PPS_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
int       PPS_PADDING = FONT_SIZE * 7;

int   LINES_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
int     LINES_PADDING = FONT_SIZE * 9;

int   TIMER_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
int     TIMER_PADDING = FONT_SIZE * 11;

const std::string clearMessage[5] = { std::string(), "SINGLE", "DOUBLE", "TRIPLE", "QUAD" };

Infor::Infor(): startTimer(false), isAllClear(false), nLine(0), piece(0), count(0), B2B(0), B2BMissing(0), spin(false), spinDraw(false) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");

    soundManager = new SoundManager();
    soundManager->loadSound("clearB2B"  , "ASSETS/sfx/clearbtb.mp3");
    soundManager->loadSound("clearLine" , "ASSETS/sfx/clearline.mp3");
    soundManager->loadSound("clearQuad" , "ASSETS/sfx/clearquad.mp3");
    soundManager->loadSound("clearSpin" , "ASSETS/sfx/clearspin.mp3");
    soundManager->loadSound("allClear"  , "ASSETS/sfx/allclear.mp3");
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
    startTimer = true;
    runningTime.restart();
    lastElapsed = sf::Time::Zero;
}

void Infor::pauseTimer() {
    lastElapsed += runningTime.getElapsedTime();
}

void Infor::unPauseTimer() {
    runningTime.restart();
}

Infor::~Infor() {
    delete soundManager;
}

void Infor::reset() { startTimer = false, nLine = 0; piece = 0, count = 0; B2B = 0; B2BMissing = 0; }

// garbage sent
uint8_t Infor::removeLine(uint8_t lines) {
    if (lines == 0) {
        // nothing
        return 0;
    }
    nLine += lines;
    return nLine;
}

void Infor::update(uint8_t lines, bool isSpin, char block, bool allClear) {
    piece++;

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

    if (allClear) {
        isAllClear = true;

        allClearTimeout.restart();
    }
}

void Infor::playSound(uint8_t lines, bool isSpin, char block, bool allClear) {
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

    if (allClear) {
        soundManager->play("allClear");
    }
}

void Infor::drawMessage(sf::RenderWindow *window, const std::string string) {
    sf::Text text(string, font, MESSAGE_FONT_SIZE);
    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, 
        INFOR_POSITION_Y + MESSAGE_PADDING
    );
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void Infor::drawCombo(sf::RenderWindow *window, const std::string string) {
    sf::Text text("COMBO", font, MESSAGE_FONT_SIZE);
    sf::Text number(string, font, COMBO_FONT_SIZE);

    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, 
        INFOR_POSITION_Y + COMBO_PADDING
    );
    number.setPosition(
        text.getPosition().x - number.getGlobalBounds().width - 15, 
        INFOR_POSITION_Y + N_COMBO_PADDING
    );
    
    float alpha = 255 * (1 - comboTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));
    number.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(number);
    window->draw(text);
}

void Infor::drawB2B(sf::RenderWindow *window) {
    sf::Text text("B2B x" + std::to_string(B2B - 1), font, B2B_FONT_SIZE);

    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, 
        INFOR_POSITION_Y + B2B_PADDING
    );
    
    float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, (alpha > 175 ? 255 : 215), 0, std::max(alpha, 175.0f)));

    window->draw(text);
}

void Infor::drawMissingB2B(sf::RenderWindow *window) {
    sf::Text text("B2B x" + std::to_string(B2BMissing - 1), font, B2B_FONT_SIZE);

    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, 
        INFOR_POSITION_Y + B2B_PADDING
    );
    
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
    sf::Text text(type + " - SPIN", font, SPIN_FONT_SIZE);

    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width, 
        INFOR_POSITION_Y
    );
    
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

void Infor::drawPPS(sf::RenderWindow *window) {  
    sf::Text title("PPS:", font, PPS_FONT_SIZE);
    title.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - title.getGlobalBounds().width,
        INFOR_POSITION_Y + PPS_PADDING
    );
    
    int milliseconds = lastElapsed.asMilliseconds() + runningTime.getElapsedTime().asMilliseconds();

    float pps = piece * 1000.0 / milliseconds;

    char ppsStr[10]; // Đủ để chứa giá trị float và null-terminator
    sprintf(ppsStr, "%.2f", pps);

    sf::Text text(ppsStr, font, LINES_FONT_SIZE);
    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width,
        INFOR_POSITION_Y + PPS_PADDING + FONT_SIZE
    );

    window->draw(title);
    window->draw(text);
}

void Infor::drawLines(sf::RenderWindow *window) {   
    sf::Text title("LINES:", font, LINES_FONT_SIZE);
    title.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - title.getGlobalBounds().width,
        INFOR_POSITION_Y + LINES_PADDING
    );
    
    sf::Text text(std::to_string(nLine), font, LINES_FONT_SIZE);
    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width,
        INFOR_POSITION_Y + LINES_PADDING + FONT_SIZE
    );

    window->draw(title);
    window->draw(text);
}

void Infor::drawTimer(sf::RenderWindow *window) {
    int milliseconds = lastElapsed.asMilliseconds() + runningTime.getElapsedTime().asMilliseconds();

    int seconds = milliseconds / 1000;
    int minutes = seconds      / 60;

    seconds      %= 60;
    milliseconds %= 1000;

    char buffer[9];  // Đủ lớn để chứa "M:SS:SSS"
    sprintf(buffer, "%01d:%02d:%03d", minutes, seconds, milliseconds);

    std::string formattedTime(buffer); // Tránh copy không cần thiết

    sf::Text title("TIME:", font, TIMER_FONT_SIZE);
    title.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - title.getGlobalBounds().width,
        INFOR_POSITION_Y + TIMER_PADDING
    );
    
    sf::Text text((startTimer) ? formattedTime : "0:00:000", font, TIMER_FONT_SIZE);
    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width,
        INFOR_POSITION_Y + TIMER_PADDING + FONT_SIZE
    );

    window->draw(title);
    window->draw(text);
}


void Infor::drawAllClear(sf::RenderWindow *window) {
    sf::Text text("ALL CLEAR", font, MESSAGE_FONT_SIZE * 1.5);

    float alpha = 255 * (1 - allClearTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    float scaleFactor = 1.0f + 0.5f * allClearTimeout.getElapsedTime().asSeconds();  // Tăng scale
    
    text.setFillColor(sf::Color(255, 255, 0, alpha));
    text.setScale(scaleFactor, scaleFactor);
    
    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH + 7 * BLOCK_SIZE - text.getGlobalBounds().width / 2, 
        INFOR_POSITION_Y + 3 * BLOCK_SIZE - text.getGlobalBounds().height / 2
    );

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
    
    drawPPS(window);

    drawLines(window);

    drawTimer(window);

    if (isAllClear and allClearTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        drawAllClear(window);
    }
    else {
        isAllClear = false;
        allClearTimeout.restart();
    }
}