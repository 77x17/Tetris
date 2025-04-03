#include "Infor_Multiplayer.hpp"

#include "CommonMap.hpp"
#include "Common.hpp"

Infor_Multiplayer::Infor_Multiplayer():Infor(), nLinesAdd(0), garbageSent(0) {}
Infor_Multiplayer::~Infor_Multiplayer() {}

// const int   FONT_SIZE = Common::BLOCK_SIZE;
// int   LEVEL_FONT_SIZE = FONT_SIZE - FONT_SIZE / 4;
// int     LINES_PADDING = FONT_SIZE * 9;

void Infor_Multiplayer::setPosition(int x, int y, int w, int gX, int gY, int gW, int gH) {
    Infor::setPosition(x, y, w);
    GARBAGE_POSITION_X = gX;
    GARBAGE_POSITION_Y = gY;
    GARBAGE_WIDTH = gW;
    GARBAGE_HEIGHT = gH;
}

void Infor_Multiplayer::reset() {
    nLinesAdd = 0;
    attactPoint = 0;
    Infor::reset();
}

int Infor_Multiplayer::getGarbage(int lines, bool spin, int B2B, int count) {
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
uint8_t Infor_Multiplayer::removeLine(uint8_t lines) {
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

    attactPoint += garbageSent;

    mtx.lock();
    uint8_t nLinesAddCurrent = __builtin_popcount(nLinesAdd);

    nLinesAdd >>= realAddLines;
    if (not spin and lines != 4)
        nLinesAdd >>= 1;
    if (getBit(nLinesAdd, 0) == 0) nLinesAdd >>= 1;

    if (nLinesAddCurrent > realAddLines) realAddLines = 0;
    else realAddLines -= nLinesAddCurrent;
    
    mtx.unlock();
    return realAddLines;
}

void Infor_Multiplayer::addLine(uint8_t lines) {
    if (lines <= 0) throw std::runtime_error("garbage push error");
    mtx.lock();
    nLinesAdd <<= (lines + 1);
    nLinesAdd |= FULLMASK(lines);
    mtx.unlock();
}

uint64_t Infor_Multiplayer::getAndRemoveLineAdd() {
    mtx.lock();
    uint64_t tmp = nLinesAdd;
    nLinesAdd = 0;
    mtx.unlock();
    return tmp;
}

void Infor_Multiplayer::drawGarbage(sf::RenderWindow *window) {
    mtx.lock();
    uint64_t tmp = nLinesAdd;
    mtx.unlock();

    int nLines = __builtin_popcount(tmp);
    sf::RectangleShape garbageBar;
    garbageBar.setSize(sf::Vector2f(GARBAGE_WIDTH * (Common::BLOCK_SIZE - Common::WIDTH_BORDER), nLines * Common::BLOCK_SIZE));
    garbageBar.setFillColor(sf::Color(255, 0, 0, 100));
    garbageBar.setPosition(GARBAGE_POSITION_X, GARBAGE_POSITION_Y - nLines * Common::BLOCK_SIZE);

    window->draw(garbageBar);

    sf::RectangleShape line;
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setSize(sf::Vector2f(GARBAGE_WIDTH * (Common::BLOCK_SIZE - Common::WIDTH_BORDER), 2));
    
    nLines = 0;
    while(tmp) {
        if (!getBit(tmp, 0)) {
            line.setPosition(GARBAGE_POSITION_X, GARBAGE_POSITION_Y - nLines * Common::BLOCK_SIZE);
            window->draw(line);
        } else nLines++;
        tmp >>= 1;
    }
}

void Infor_Multiplayer::drawGarbageSent(sf::RenderWindow *window) {
    sf::Text text(std::to_string(garbageSent), font, 50);
    text.setPosition(sf::Vector2f(GARBAGE_POSITION_X - text.getGlobalBounds().width - Common::BLOCK_SIZE, 
                                  GARBAGE_POSITION_Y - text.getGlobalBounds().height - Common::BLOCK_SIZE / 2));

    float alpha = 255 * (1 - garbageSentTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void Infor_Multiplayer::drawAPM(sf::RenderWindow *window) {  
    sf::Text title("APM:", font, LEVEL_FONT_SIZE);
    
    title.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - title.getGlobalBounds().width,
        INFOR_POSITION_Y + LINES_PADDING
    );
    
    int milliseconds = lastElapsed.asMilliseconds() + runningTime.getElapsedTime().asMilliseconds();
    
    float pps = 60000.0 / milliseconds * attactPoint;
    
    char apmStr[10]; // Đủ để chứa giá trị float và null-terminator
    sprintf(apmStr, "%.2f", pps);
    
    sf::Text text(apmStr + (std::string)"/min", font, LEVEL_FONT_SIZE);
    sf::Text number(std::to_string(attactPoint), font, LEVEL_FONT_SIZE * 2);

    text.setPosition(
        INFOR_POSITION_X + INFOR_WIDTH - text.getGlobalBounds().width,
        INFOR_POSITION_Y + LINES_PADDING + FONT_SIZE
    );
    number.setPosition(
        text.getPosition().x - number.getGlobalBounds().width - 15, 
        text.getPosition().y - number.getGlobalBounds().height / 2
    );

    window->draw(title);
    window->draw(text);
    window->draw(number);
}

void Infor_Multiplayer::draw(sf::RenderWindow *window, int mode = 1) {
    Infor::draw(window, mode);

    drawGarbage(window);

    if (garbageSentTimeout.getElapsedTime().asSeconds() < TIME_OUT) {
        if (garbageSent != 0) {
            drawGarbageSent(window);
        }
    }
    else {
        garbageSent = 0;
    }

    drawAPM(window);
}