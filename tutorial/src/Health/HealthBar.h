#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include <string>

enum class BarType { HEALTH, MANA };

class HealthBar {
public:
    HealthBar(SDL_Renderer* renderer, int x, int y, int width, int height, const std::string& texturePath, BarType type);
    ~HealthBar();
    
    void RestartLifePoints(){m_lifepoints = 3;}
    int GetLifePoints(){return m_lifepoints;}
    void Update(float value);
    void Render();

private:
    SDL_Renderer* m_Renderer;
    SDL_Rect m_BarRect;
    SDL_Rect m_FillRect;
    SDL_Rect m_TextureRect;
    SDL_Texture* m_Texture;
    float m_Value;
    int m_lifepoints = 3;
    bool state_respawn = true;
    BarType m_Type;
};

#endif // HEALTHBAR_H
