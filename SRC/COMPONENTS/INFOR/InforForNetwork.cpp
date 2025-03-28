#include "InforForNetwork.hpp"

#include "Common.hpp"

InforForNetwork::InforForNetwork():Infor(), nLinesAdd(0), garbageSent(0) {}
InforForNetwork::~InforForNetwork() {}

void InforForNetwork::setPosition(int x, int y, int w, int gX, int gY, int gW, int gH) {
    Infor::setPosition(x, y, w);
    GARBAGE_POSITION_X = gX;
    GARBAGE_POSITION_Y = gY;
    GARBAGE_WIDTH = gW;
    GARBAGE_HEIGHT = gH;
}

void InforForNetwork::reset() {
    nLinesAdd = 0;
    Infor::reset();
}

int InforForNetwork::getGarbage(int lines, bool spin, int B2B, int count) {
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
uint8_t InforForNetwork::removeLine(uint8_t lines) {
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

void InforForNetwork::addLine(uint8_t lines) {
    if (lines <= 0) throw std::runtime_error("garbage push error");
    mtx.lock();
    nLinesAdd <<= (lines + 1);
    nLinesAdd |= FULLMASK(lines);
    mtx.unlock();
}

uint64_t InforForNetwork::getAndRemoveLineAdd() {
    mtx.lock();
    uint64_t tmp = nLinesAdd;
    nLinesAdd = 0;
    mtx.unlock();
    return tmp;
}

void InforForNetwork::drawGarbage(sf::RenderWindow *window) {
    mtx.lock();
    uint64_t tmp = nLinesAdd;
    mtx.unlock();

    int nLines = __builtin_popcount(tmp);
    sf::RectangleShape garbageBar;
    garbageBar.setSize(sf::Vector2f(GARBAGE_WIDTH * (BLOCK_SIZE - WIDTH_BORDER), nLines * BLOCK_SIZE));
    garbageBar.setFillColor(sf::Color(255, 0, 0, 100));
    garbageBar.setPosition(GARBAGE_POSITION_X, GARBAGE_POSITION_Y - nLines * BLOCK_SIZE);

    window->draw(garbageBar);

    sf::RectangleShape line;
    line.setFillColor(sf::Color(255, 255, 255, 200)); // White
    line.setSize(sf::Vector2f(GARBAGE_WIDTH * (BLOCK_SIZE - WIDTH_BORDER), 2));
    
    nLines = 0;
    while(tmp) {
        if (!getBit(tmp, 0)) {
            line.setPosition(GARBAGE_POSITION_X, GARBAGE_POSITION_Y - nLines * BLOCK_SIZE);
            window->draw(line);
        } else nLines++;
        tmp >>= 1;
    }
}

void InforForNetwork::drawGarbageSent(sf::RenderWindow *window) {
    sf::Text text(std::to_string(garbageSent), font, 50);
    text.setPosition(sf::Vector2f(GARBAGE_POSITION_X - text.getGlobalBounds().width - BLOCK_SIZE, 
                                  GARBAGE_POSITION_Y - text.getGlobalBounds().height - BLOCK_SIZE));

    float alpha = 255 * (1 - garbageSentTimeout.getElapsedTime().asSeconds() / TIME_OUT);
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window->draw(text);
}

void InforForNetwork::draw(sf::RenderWindow *window) {
    Infor::draw(window);

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
void InforForNetwork::compress(sf::Packet &packet) {
    packet << spin << B2B << count;
}