#include "HealthBar.h"
#include "string.h"
#include <iostream>
HealthBar::HealthBar(SDL_Renderer* renderer, int x, int y, int width, int height, const std::string& texturePath, BarType type)
    : m_Renderer(renderer), m_Value(1.0f), m_Type(type) {

    // Load texture
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load health bar texture: %s", SDL_GetError());
        m_Texture = nullptr;
    } else {
        m_Texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    // Set up bar position and size
    m_BarRect = { x, y, width, height };
    m_FillRect = { x + 2, y + 2, width - 4, height - 4 };

    // Set texture position (left of the bar)
    m_TextureRect = { x - 40, y - 15, 40, 40 };
}

HealthBar::~HealthBar() {
    if (m_Texture) {
        SDL_DestroyTexture(m_Texture);
    }
}

void HealthBar::Update(float value) {
    // Clamp value between 0 and 1
    m_Value = value;
    if (m_Value < 0.0f) m_Value = 0.0f;
    if (m_Value > 1.0f) m_Value = 1.0f;
    
    if (m_Value == 0.0f && state_respawn == true){
        state_respawn = false;
    }
    else if(m_Value == 1.0f && state_respawn == false){
        state_respawn = true;
        --m_lifepoints;
    }
    // Update the width of the filled portion
    m_FillRect.w = static_cast<int>((m_BarRect.w - 4) * m_Value);
   
}

void HealthBar::Render() {
    if (m_Texture) {
        SDL_RenderCopy(m_Renderer, m_Texture, nullptr, &m_TextureRect);
    }

    // Draw the outer border (black)
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_Renderer, &m_BarRect);

    // Set fill color based on health points
    Uint8 r = 255, g = 0, b = 0;
    Uint8 lost_r = 255, lost_g = 255, lost_b = 255; // Default lost portion to white

    if (m_Type == BarType::HEALTH) {
        if (m_lifepoints == 3) {  // Green -> Lost portion is Yellow
            r = 0;
            g = 255;
            b = 0;
            lost_r = 255;
            lost_g = 255;
            lost_b = 0;
        } else if (m_lifepoints == 2) {  // Yellow -> Lost portion is Red
            r = 255;
            g = 255;
            b = 0;
            lost_r = 255;
            lost_g = 0;
            lost_b = 0;
        } else if (m_lifepoints == 1) {  // Red -> Lost portion is White
            r = 255;
            g = 0;
            b = 0;
            lost_r = 255;
            lost_g = 255;
            lost_b = 255;
        }
    } else {
        r = 0; g = 0; b = 255;
    }

    // Draw the filled portion
    SDL_SetRenderDrawColor(m_Renderer, r, g, b, 255);
    SDL_RenderFillRect(m_Renderer, &m_FillRect);

    // Draw the lost portion with correct color
    SDL_Rect lostHealthRect = {
        m_FillRect.x + m_FillRect.w,
        m_FillRect.y,
        (m_BarRect.w - 4) - m_FillRect.w,
        m_FillRect.h
    };
    SDL_SetRenderDrawColor(m_Renderer, lost_r, lost_g, lost_b, 255);
    SDL_RenderFillRect(m_Renderer, &lostHealthRect);
}
