#include "Item.h"
#include "Engine.h"
#include "Camera.h"

Item::Item(const std::string& textureID, int x, int y, int width, int height, ItemType type)
    : m_Type(type) {
    m_Texture = TextureManager::GetInstance()->GetTexture(textureID);
    m_Collider = {x, y, width, height};
}

void Item::Render() {
    int offsetX = Camera::GetInstance()->GetPosition().X;
    int offsetY = Camera::GetInstance()->GetPosition().Y;
    
    SDL_Rect dstRect = { m_Collider.x - offsetX, m_Collider.y - offsetY, m_Collider.w, m_Collider.h };
    SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), m_Texture, nullptr, &dstRect);
}
