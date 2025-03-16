#include "SoundManager.hpp"

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

void SoundManager::loadSound(const std::string& name, const std::string& filePath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath)) return;
    buffers[name] = buffer;
    sounds[name].setBuffer(buffers[name]);
}

void SoundManager::play(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].play();
    }
}