#pragma once

#include <unordered_map>
#include <SFML/Audio.hpp>

class SoundManager {
private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::unordered_map<std::string, sf::Sound> sounds;

public:
    SoundManager();
    ~SoundManager();
    void loadSound(const std::string& name, const std::string& filePath);
    void play(const std::string& name);

};
    