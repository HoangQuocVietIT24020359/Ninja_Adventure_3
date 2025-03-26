#include "Checkpoint.h"

Checkpoint::Checkpoint(SDL_Renderer* renderer, int x, int y, int width, int height)
    : m_Renderer(renderer), m_Activated(false), m_Frame(0), m_AnimationTimer(0), m_Width(width), m_Height(height)
, m_X(x), m_Y(y){
    m_Collider = {x, y, width, height};
    TextureManager::GetInstance()->Load("checkpoint", "assets/checkpoint/flag.png");
    m_Texture = TextureManager::GetInstance()->GetTexture("checkpoint");
}

void Checkpoint::Update(float dt) {
    if (m_Activated) {
        m_AnimationTimer += dt;
        if (m_AnimationTimer > 0.1f) {
            m_Frame = (m_Frame + 1) % 10; 
            m_AnimationTimer = 0;
        }
    }
}

void Checkpoint::Render(int offsetX, int offsetY) {
    SDL_Rect srcRect = {m_Frame * 202, 0, 202, 324};
    SDL_Rect dstRect = {m_Collider.x - offsetX, m_Collider.y - offsetY, m_Width, m_Height};

    SDL_RenderCopy(m_Renderer, m_Texture, &srcRect, &dstRect);
}

