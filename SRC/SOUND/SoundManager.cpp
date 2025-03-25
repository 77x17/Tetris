#include "SoundManager.hpp"

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

constexpr float AUDIO_PADDING = 5.0f;

void SoundManager::loadSound(const std::string& name, const std::string& filePath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath)) return;
    buffers[name] = buffer;
    sounds[name].setBuffer(buffers[name]);
}

void SoundManager::play(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].setVolume(volume);
        sounds[name].play();
    }
}

void SoundManager::playMusic() {
    sounds["music"].setLoop(true);
    sounds["mucis"].setVolume(musicVolume);
    sounds["music"].play();
}

void SoundManager::stopMusic() {
    sounds["music"].stop();
}

float SoundManager::getVolume() {
    return volume;
}

float SoundManager::getMusicVolume() {
    return musicVolume;
}

void SoundManager::setVolume(const float &newVolume) {
    volume = newVolume;
}

void SoundManager::increaseVolume() {
    volume = (volume >= 100 - AUDIO_PADDING ? 100 : volume + AUDIO_PADDING);
}

void SoundManager::decreaseVolume() {
    volume = (volume <= AUDIO_PADDING ? 0 : volume - AUDIO_PADDING);
}

void SoundManager::increaseMusicVolume() {
    musicVolume = (musicVolume >= 100 - AUDIO_PADDING ? 100 : musicVolume + AUDIO_PADDING);
}

void SoundManager::decreaseMusicVolume() {
    musicVolume = (musicVolume <= AUDIO_PADDING ? 0 : musicVolume - AUDIO_PADDING);
}