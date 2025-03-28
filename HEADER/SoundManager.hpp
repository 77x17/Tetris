#pragma once

#include <unordered_map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class SoundManager {
private:
    sf::Font font;

    int AUDIO_POSITION_X;
    int AUDIO_POSITION_Y;
    int AUDIO_WIDTH;
    int AUDIO_HEIGHT;

    // SFX
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::unordered_map<std::string, sf::Sound> sounds;

    static std::unordered_map<std::string, sf::SoundBuffer> musicBuffers;
    static std::unordered_map<std::string, sf::Sound> musicSounds;

    static float volume;
    static float musicVolume;

public:                         
    SoundManager();
    ~SoundManager();

    void setPosition(int AUDIO_POSITION_X, int AUDIO_POSITION_Y, int AUDIO_WIDTH, int AUDIO_HEIGHT);

    void loadSound(const std::string &name, const std::string &filePath);
    
    static void loadMusicSound(const std::string &name, const std::string &filePath);
    void play(const std::string &name);
    void pause(const std::string &name);
    void unPause(const std::string &name);

    void drawSfx(sf::RenderWindow *window);

    static void  playMusic(const std::string &name);
    static void  pauseMusic(const std::string &name);
    static void  unPauseMusic(const std::string &name);
    static void  stopMusic(const std::string &name);
    static float getVolume();
    static float getMusicVolume();
    static void  setVolume(const float &volume);
    static void  setMusicVolume();
    static void  increaseVolume();
    static void  decreaseVolume();
    static void  increaseMusicVolume();
    static void  decreaseMusicVolume();
};