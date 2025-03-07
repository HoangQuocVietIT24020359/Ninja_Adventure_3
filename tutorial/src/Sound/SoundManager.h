#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SDL2_mixer/SDL_mixer.h"
#include <string>
#include <map>

class SoundManager {
public:
    static SoundManager* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new SoundManager();
        }
        return s_Instance;
    }
    
    bool Init();
    void LoadMusic(const std::string& id, const std::string& path);
    void LoadSoundEffect(const std::string& id, const std::string& path);
    void PlayMusic(const std::string& id, int loops = -1);
    void PlaySound(const std::string& id, int loops = 0);
    void Clean();
    
private:
    SoundManager() {}
    static SoundManager* s_Instance;
    std::map<std::string, Mix_Music*> m_Music;
    std::map<std::string, Mix_Chunk*> m_SoundEffects;
};

#endif
