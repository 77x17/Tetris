#include "Tetris.hpp"

#include "TetrisOnePlayer.hpp"
#include "TetrisTwoPlayer.hpp"
#include "TetrisVsBot.hpp"
#include "SoundManager.hpp"
#include "Menu.hpp"
#include "Scene.hpp"

#include <SFML/Graphics.hpp>
#include <windows.h>

const int WINDOW_WIDTH  = 1100;
const int WINDOW_HEIGHT = 600;

float SoundManager::volume      = 0.0f;
float SoundManager::musicVolume =  0.0f;
std::unordered_map<std::string, sf::SoundBuffer> SoundManager::musicBuffers = std::unordered_map<std::string, sf::SoundBuffer>();
std::unordered_map<std::string, sf::Sound>       SoundManager::musicSounds  = std::unordered_map<std::string, sf::Sound>();

Tetris::Tetris() {
    // sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // window = new sf::RenderWindow(desktop, "Tetris", sf::Style::None); // Không viền
    // window = new sf::RenderWindow(desktop, "Tetris"); // Không viền
    // window->setPosition(sf::Vector2i(0, 0)); // Đặt vị trí góc trên cùng bên trái

    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetris");
    scene  = new Scene(window);
}

Tetris::~Tetris() {
    delete scene; scene = nullptr;
    delete window; window = nullptr;
}

void Tetris::start() {
    while (window->isOpen()) {       
        STATUS_CODE gameType = scene->drawMenu(window);
        
        STATUS_CODE screenStatus = STATUS_CODE::QUIT;
        switch (gameType) {
            case STATUS_CODE::PRACTICE:
                screenStatus = TetrisOnePlayer(window, scene).start();
                break;
            case STATUS_CODE::VERSUSBOT:
                screenStatus = TetrisVsBot(window, scene).start();
                break;
            case STATUS_CODE::MULTIPLAYER_SERVER:
                screenStatus = TetrisTwoPlayer(window, scene, true).start();
                break;
            case STATUS_CODE::MULTIPLAYER_CLIENT:
                screenStatus = TetrisTwoPlayer(window, scene, false).start();
                break;
            case STATUS_CODE::QUIT:
                window->close();
                break;
            default:
                throw std::runtime_error("[Tetris.cpp] - start(): STATUS_CODE error");
        }
        
        switch (screenStatus) {
            case STATUS_CODE::MENU:
                break;
            case STATUS_CODE::QUIT:
                window->close();
                break;
            default:
                throw std::runtime_error("[Tetris.cpp] - start(): STATUS_CODE error");
        }
    }
}
