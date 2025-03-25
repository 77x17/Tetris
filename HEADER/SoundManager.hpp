#pragma once

#include <unordered_map>
#include <SFML/Audio.hpp>

class SoundManager {
private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::unordered_map<std::string, sf::Sound> sounds;

    static float volume;
    static float musicVolume;

public:
    SoundManager();
    ~SoundManager();
    void loadSound(const std::string &name, const std::string &filePath);
    void play(const std::string &name);
    void playMusic();
    void stopMusic();
    static float getVolume();
    static float getMusicVolume();
    static void  setVolume(const float &volume);
    static void  increaseVolume();
    static void  decreaseVolume();
    static void  increaseMusicVolume();
    static void  decreaseMusicVolume();
};