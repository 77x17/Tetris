#include "Game.hpp"

#include "Tetris_Practice.hpp"
#include "Tetris_Survival.hpp"
#include "Tetris_Multiplayer.hpp"
#include "Tetris_VersusBot.hpp"
#include "SoundManager.hpp"
#include "Menu.hpp"
#include "Scene.hpp"
#include "Common.hpp"

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

int Common::BLOCK_SIZE   = 20;
int Common::WIDTH_BORDER = 5;

int Common::WIDTH_MAP  = 10;
int Common::HEIGHT_MAP = 24;


const int WINDOW_WIDTH  = 1100;
const int WINDOW_HEIGHT = 600;

float SoundManager::volume      = 50.0f;
float SoundManager::musicVolume =  0.0f;
std::unordered_map<std::string, sf::SoundBuffer> SoundManager::musicBuffers = std::unordered_map<std::string, sf::SoundBuffer>();
std::unordered_map<std::string, sf::Sound>       SoundManager::musicSounds  = std::unordered_map<std::string, sf::Sound>();

Game::Game() {
    // sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // window = new sf::RenderWindow(desktop, "Game", sf::Style::None); // Không viền
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game", sf::Style::Close);
    scene  = new Scene(window);

    window->setFramerateLimit(60);
}

Game::~Game() {
    delete scene; scene = nullptr;
    delete window; window = nullptr;
}

void Game::start() {
    MENU_CODE menuCode = MENU_CODE::MAIN;
    // bool isFullscreen = false;
    
    while (true) {
        // bool run = false;
    //     while (not run) {
    //         sf::Event event;
    //         while (window->pollEvent(event)) {
    //             if (event.type == sf::Event::KeyPressed) {
    //                 if (event.key.code == sf::Keyboard::F11) {
    //                     delete window;
    //                     delete scene;
                        
    //                     if (isFullscreen) {
    //                         window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game", sf::Style::Close);
    //                         scene  = new Scene(window);
    
    //                         Common::BLOCK_SIZE = 20;
                            
    //                         isFullscreen = false;
    //                     }
    //                     else {
    //                         sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    //                         window = new sf::RenderWindow(desktop, "Game", sf::Style::None); // Không viền
    //                         scene  = new Scene(window);
                            
    //                         Common::BLOCK_SIZE = 25;
    
    //                         isFullscreen = true;
    //                     }
    
    //                     window->setFramerateLimit(60);
    //                 }
    //                 else if (event.key.code == sf::Keyboard::Escape) {
    //                     run = true;
    //                 }
    //             }
    //         }
    //     }

        STATUS_CODE gameType = scene->drawMenu(window, menuCode);
        // STATUS_CODE gameType = STATUS_CODE::VERSUSBOT;

        STATUS_CODE screenStatus = STATUS_CODE::QUIT;
        switch (gameType) {
            case STATUS_CODE::PRACTICE: {
                screenStatus = Tetris_Practice(window, scene).start();

                if (screenStatus == STATUS_CODE::MENU) {
                    menuCode = MENU_CODE::SINGLEPLAYER;
                }

                break;
            }
            case STATUS_CODE::SURVIVAL: {
                screenStatus = Tetris_Survival(window, scene).start();

                if (screenStatus == STATUS_CODE::MENU) {
                    menuCode = MENU_CODE::SINGLEPLAYER;
                }

                break;
            }
            case STATUS_CODE::VERSUSBOT: {
                screenStatus = Tetris_VersusBot(window, scene).start();
                
                if (screenStatus == STATUS_CODE::MENU) {
                    menuCode = MENU_CODE::SINGLEPLAYER;
                }

                break;
            }
            case STATUS_CODE::MULTIPLAYER_SERVER: {
                screenStatus = Tetris_Multiplayer(window, scene, true).start();

                if (screenStatus == STATUS_CODE::MENU) {
                    menuCode = MENU_CODE::MULTIPLAYER;
                }

                break;
            }
            case STATUS_CODE::MULTIPLAYER_CLIENT: {
                screenStatus = Tetris_Multiplayer(window, scene, false).start();

                if (screenStatus == STATUS_CODE::MENU) {
                    menuCode = MENU_CODE::MULTIPLAYER;
                }

                break;
            }
            case STATUS_CODE::QUIT: {
                window->close();

                break;
            }
            default: {
                throw std::runtime_error("[Game.cpp] - start(): STATUS_CODE error");
            }
        }
        
        if (screenStatus == STATUS_CODE::QUIT) {
            window->close();
            
            break;
        }
    }
}
