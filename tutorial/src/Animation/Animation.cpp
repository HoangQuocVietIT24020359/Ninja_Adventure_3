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
    m_SpriteRow = SpriteRow;
    m_FrameCount = frameCount;
    m_AnimSpeed = animSpeed;
    m_Flip = flip;
}
