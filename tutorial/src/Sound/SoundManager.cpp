#include "SoundManager.h"
#include <SDL2/SDL.h>
#include <iostream>
SoundManager* SoundManager::s_Instance = nullptr;

bool SoundManager::Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Audio Initialization Failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Initialization Failed: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void SoundManager::LoadMusic(const std::string& id, const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music == nullptr) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }
    m_Music[id] = music;
}

void SoundManager::LoadSoundEffect(const std::string& id, const std::string& path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        std::cerr << "Failed to load sound effect: " << Mix_GetError() << std::endl;
        return;
    }
    m_SoundEffects[id] = sound;
}

void SoundManager::PlayMusic(const std::string& id, int loops) {
    if (m_Music[id]) {
        Mix_PlayMusic(m_Music[id], loops);
    }
}

void SoundManager::PlaySound(const std::string& id, int loops) {
    if (m_SoundEffects[id]) {
        Mix_PlayChannel(-1, m_SoundEffects[id], loops);
    }
}

void SoundManager::Clean() {
    for (auto& music : m_Music) {
        Mix_FreeMusic(music.second);
    }
    m_Music.clear();
    
    for (auto& sound : m_SoundEffects) {
        Mix_FreeChunk(sound.second);
    }
    m_SoundEffects.clear();
    
    Mix_CloseAudio();
}
