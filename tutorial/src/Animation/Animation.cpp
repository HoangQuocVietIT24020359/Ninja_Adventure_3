#include "Animation.h"
#include "TextureManager.h"

void Animation::Update(){
    m_SpriteFrame = (SDL_GetTicks()/m_AnimSpeed) % m_FrameCount;
}

void Animation::Draw(float x, float y, int spriteWidtd, int spriteHeight){
    TextureManager::GetInstance()->DrawFrame(m_textureID, x, y, spriteWidtd, spriteHeight, m_SpriteRow, m_SpriteFrame, m_Flip);
}

void Animation::SetProps(string textureID, int SpriteRow, int frameCount, int animSpeed, SDL_RendererFlip flip){
    m_textureID = textureID;
    m_SpriteRow = SpriteRow; // The row in the sprite sheet where the animation frames are located
    m_FrameCount = frameCount; // Total number of frames in the animation
    m_AnimSpeed = animSpeed;
    m_Flip = flip; // Flip type (e.g., SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL)
}
