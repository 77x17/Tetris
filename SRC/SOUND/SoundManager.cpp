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

void SoundManager::loadMusicSound(const std::string& name, const std::string& filePath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath)) return;
    musicBuffers[name] = buffer;
    musicSounds[name].setBuffer(musicBuffers[name]);
}

void SoundManager::play(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].setVolume(volume);
        sounds[name].play();
    }
}

void SoundManager::pause(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        if (sounds[name].getStatus() == sf::Music::Status::Playing) {
            sounds[name].pause();
        }
    }
}

void SoundManager::unPause(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        if (sounds[name].getStatus() == sf::Music::Status::Paused) {
            sounds[name].play();
        }
    }
}


void SoundManager::playMusic(const std::string& name) {
    musicSounds[name].setLoop(true);
    musicSounds[name].setVolume(musicVolume);
    musicSounds[name].play();
}

void SoundManager::pauseMusic(const std::string& name) {
    if (musicSounds[name].getStatus() == sf::Music::Status::Playing) {
        musicSounds[name].pause();
    }
}

void SoundManager::unPauseMusic(const std::string& name) {
    if (musicSounds[name].getStatus() == sf::Music::Status::Paused) {
        musicSounds[name].play();
    }
}

void SoundManager::stopMusic(const std::string& name) {
    musicSounds[name].stop();
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

void SoundManager::setMusicVolume() {
    for (auto &music : musicSounds) {
        music.second.setVolume(musicVolume);        
    }
}

void SoundManager::increaseMusicVolume() {
    musicVolume = (musicVolume >= 100 - AUDIO_PADDING ? 100 : musicVolume + AUDIO_PADDING);

    setMusicVolume();
}

void SoundManager::decreaseMusicVolume() {
    musicVolume = (musicVolume <= AUDIO_PADDING ? 0 : musicVolume - AUDIO_PADDING);

    setMusicVolume();
}