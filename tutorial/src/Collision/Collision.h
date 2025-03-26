// Collision.h
#ifndef COLLISION_H
#define COLLISION_H

#include "SDL2/SDL.h"
#include <iostream>

class Collision {
public:
    static bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
        return SDL_HasIntersection(&rect1, &rect2);
    }
};

#endif // COLLISION_H
