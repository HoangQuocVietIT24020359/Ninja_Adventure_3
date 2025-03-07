#ifndef ITEM_H
#define ITEM_H

#include "TextureManager.h"
#include "Collision.h"
#include "SDL2/SDL.h"

enum class ItemType { HEALTH, MANA };

class Item {
public:
    Item(const std::string& textureID, int x, int y, int width, int height, ItemType type);


    void Render();
    SDL_Rect GetCollider() const { return m_Collider; }
    ItemType GetType() const { return m_Type; }

private:
    SDL_Texture* m_Texture;
    SDL_Rect m_Collider;
    ItemType m_Type;
};

#endif // ITEM_H
