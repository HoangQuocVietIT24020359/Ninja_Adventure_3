#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "SDL2/SDL.h"
#include "vector2D.h"
#include "TextureManager.h"
#include "Collision.h"

class Checkpoint {
public:
    Checkpoint(SDL_Renderer* renderer, int x, int y, int width = 202, int height = 324);
    void Update(float dt);
    void Render(int offsetX, int offsetY);
    SDL_Rect GetCollider() { return m_Collider; }
    void Activate() { m_Activated = true; }
    bool IsActivated() { return m_Activated; }
    int GetX() const { return m_X; }
    int GetY() const { return m_Y - 75; }


private:
    SDL_Renderer* m_Renderer;
    SDL_Rect m_Collider;
    SDL_Texture* m_Texture;
    bool m_Activated;
    int m_Frame;
    float m_AnimationTimer;
    int m_Width, m_Height;
    int m_X, m_Y;
};

#endif
