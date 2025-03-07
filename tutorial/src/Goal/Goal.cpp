#include "Goal.h"

Goal::Goal(SDL_Renderer* renderer, int x, int y, int width, int height)
    : m_Renderer(renderer), m_Activated(false), m_Frame(0), m_AnimationTimer(0), m_Width(width), m_Height(height),
      m_X(x), m_Y(y) {
    m_Collider = {x, y, width, height};
    TextureManager::GetInstance()->Load("goal", "assets/checkpoint/door.png");
    m_Texture = TextureManager::GetInstance()->GetTexture("goal");
}

void Goal::Render(int offsetX, int offsetY) {
    SDL_Rect srcRect = {0, 0, 536, 466};  // Original size of the texture

    // Scale Down (Adjust These Values)
    float scaleFactor = 0.3f; // Adjust scale factor (e.g., 0.5 for half size)
    int newWidth = static_cast<int>(m_Width * scaleFactor);
    int newHeight = static_cast<int>(m_Height * scaleFactor);

    SDL_Rect dstRect = {m_Collider.x - offsetX, m_Collider.y - offsetY, newWidth, newHeight};

    SDL_RenderCopy(m_Renderer, m_Texture, &srcRect, &dstRect);
}
