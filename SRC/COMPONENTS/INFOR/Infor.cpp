#include "Infor.hpp"

#include "SoundManager.hpp"
#include "Common.hpp"

const int FONT_SIZE = 25;

constexpr float TIME_OUT = 2.0f;
const std::string clearMessage[5] = { std::string(), "SINGLE", "DOUBLE", "TRIPLE", "QUAD" };

Infor::Infor(int x, int y, int w, int aX, int aY, int aW, int aH, int gX, int gY, int gW, int gH) : 
        INFOR_POSITION_X(x)   , INFOR_POSITION_Y(y)   , INFOR_WIDTH(w)   , 
        AUDIO_POSITION_X(aX)  , AUDIO_POSITION_Y(aY)  , AUDIO_WIDTH(aW)  , AUDIO_HEIGHT(aH)  ,
        GARBAGE_POSITION_X(gX), GARBAGE_POSITION_Y(gY), GARBAGE_WIDTH(gW), GARBAGE_HEIGHT(gH),
        nLine(0), count(0), B2B(0), B2BMissing(0), spin(false), nLinesAdd(0), garbageSent(0) {
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
    nLine = 0; count = 0; B2B = 0; B2BMissing = 0; nLinesAdd = 0;
}

int Infor::getGarbage(int lines, bool spin, int B2B, int count) {
    int result = 0;
    if (spin) { 
        result = lines * 2;
    }
    else {
        if (lines < 4) { 
            result += lines - 1;
        }
        else {
            result += lines;
        }
    }

    // B2B Calc
    B2B--;
    if (B2B <= 0) {
        // nothing
    }
    else if (1 <= B2B and B2B <= 2) {
        result += 1;
    }
    else if (3 <= B2B and B2B <= 7) {
        result += 2;
    }
    else if (8 <= B2B and B2B <= 23) {
        result += 3;
    }
    else if (24 <= B2B and B2B <= 66) {
        result += 4;
    }
    else if (67 <= B2B) {
        result += 5;
    }

    // combo
    count--;
    if (spin and lines >= 2) {
        result += count;
    }
    else if (count <= 1) {
        // nothing
    }
    else if (2 <= count and count <= 5) {
        result += 1;
    }
    else if (6 <= count and count <= 15) {
        result += 2;
    }
    else if (16 <= count and count <= 20) {
        result += 3;
    }

    return result;
}

// garbage sent
uint8_t Infor::removeLine(uint8_t lines) {
    if (lines == 0) {
        // nothing
        return 0;
    }

    garbageSent = getGarbage(lines, spin, B2B, count);
    if (garbageSent != 0) {
        garbageSentTimeout.restart();
    }

    nLine += lines;
    int realAddLines = garbageSent;

    if (not spin and lines != 4) {
        realAddLines += 1;
    }

    mtx.lock();
    uint8_t nLinesAddCurrent = __builtin_popcount(nLinesAdd);

    nLinesAdd >>= realAddLines;
    if (getBit(nLinesAdd, 0) == 0) nLinesAdd >>= 1;

    if (nLinesAddCurrent > realAddLines) realAddLines = 0;
    else realAddLines -= nLinesAddCurrent;
    
    mtx.unlock();
    return realAddLines;
}
#include <iostream>
// garbage receive
void Infor::addLine(uint8_t lines) {
    if (lines <= 0) throw std::runtime_error("garbage push error");
    // Only use infor in decleration and donot access outer infor.
    // lines = getGarbage(lines, spin, B2B, count);

    if (lines) {
        mtx.lock();
        nLinesAdd <<= (lines + 1);
        nLinesAdd |= FULLMASK(lines);
        mtx.unlock();
    }
}

// void Infor::addLine(uint8_t lines, Infor* infor) {
//     addLine(lines, infor->spin, infor->B2B, infor->count);
// }

uint64_t Infor::getAndRemoveLineAdd() {
    mtx.lock();
    uint64_t tmp = nLinesAdd;
    nLinesAdd = 0;
    mtx.unlock();
    return tmp;
}

void Infor::update(uint8_t lines, bool isSpin, char block) {
    if (isSpin) {
        spin      = isSpin;
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

void Infor::drawAudio(sf::RenderWindow *window, const float &volume) {
    sf::Text text("AUDIO", font, BLOCK_SIZE - BLOCK_SIZE / 3);
    text.setPosition(AUDIO_POSITION_X, AUDIO_POSITION_Y - BLOCK_SIZE - BLOCK_SIZE / 6);
    window->draw(text);

    sf::RectangleShape line;
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White

    // Upper - lower line
    line.setSize(sf::Vector2f(AUDIO_WIDTH * BLOCK_SIZE + WIDTH_BORDER + WIDTH_BORDER, WIDTH_BORDER));
    line.setPosition(AUDIO_POSITION_X - WIDTH_BORDER, AUDIO_POSITION_Y - WIDTH_BORDER);
    window->draw(line);
    line.setPosition(AUDIO_POSITION_X - WIDTH_BORDER, AUDIO_POSITION_Y + AUDIO_HEIGHT * BLOCK_SIZE);
    window->draw(line);

    sf::RectangleShape volumeBar;
    volumeBar.setSize(sf::Vector2f(AUDIO_WIDTH * BLOCK_SIZE * (volume / 100.0f), AUDIO_HEIGHT * BLOCK_SIZE));  // Width proportional to volume
    volumeBar.setFillColor(sf::Color::White);
    volumeBar.setPosition(AUDIO_POSITION_X, AUDIO_POSITION_Y);

    window->draw(volumeBar);
}

void Infor::drawGarbage(sf::RenderWindow *window) {
    mtx.lock();
    uint64_t tmp = nLinesAdd;
    mtx.unlock();

    int nLines = __builtin_popcount(tmp);
    sf::RectangleShape garbageBar;
    garbageBar.setSize(sf::Vector2f(GARBAGE_WIDTH * (BLOCK_SIZE - WIDTH_BORDER), nLines * BLOCK_SIZE));
    garbageBar.setFillColor(sf::Color(255, 0, 0, 100));
    garbageBar.setPosition(GARBAGE_POSITION_X, GARBAGE_POSITION_Y - nLines * BLOCK_SIZE);

    sf::RectangleShape line;
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setSize(sf::Vector2f(GARBAGE_WIDTH * (BLOCK_SIZE - WIDTH_BORDER), 2));
    
    for (int i = 0; i + 1 < 64; i++) 
        if (getBit(tmp, i)) {
            nLines--;
            if (getBit(tmp, i + 1) == 0){
                line.setPosition(GARBAGE_POSITION_X, GARBAGE_POSITION_Y - nLines * BLOCK_SIZE);
                window->draw(line);
            }
        }


    window->draw(garbageBar);
}

void Infor::drawGarbageSent(sf::RenderWindow *window) {
    sf::Text text(std::to_string(garbageSent), font, 50);
    text.setPosition(sf::Vector2f(GARBAGE_POSITION_X - text.getGlobalBounds().width - BLOCK_SIZE, 
                                  GARBAGE_POSITION_Y - text.getGlobalBounds().height - BLOCK_SIZE));

    float alpha = 255 * (1 - garbageSentTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void Infor::draw(sf::RenderWindow *window) {
    if (message != std::string() and timeout.getElapsedTime().asSeconds() < TIME_OUT) {
        drawMessage(window, message);
    }

    if (spin and spinTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        drawSpin(window);
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

    drawGarbage(window);

    if (garbageSentTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        if (garbageSent != 0) {
            drawGarbageSent(window);
        }
    }
    else {
        garbageSent = 0;
    }
}

// Guarantee inorder.
void Infor::compress(sf::Packet &packet) {
    packet << spin << B2B << count;
}