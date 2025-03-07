// Collision.h
#ifndef COLLISION_H
#define COLLISION_H

#include "SDL2/SDL.h"
#include <iostream>

class Collision {
public:
    static bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
        // SDL_Rect is a struct, not a pointer, so it can never be nullptr.
               // Instead, check if width and height are zero (indicating an invalid rectangle).
               if (rect1.w == 0 || rect1.h == 0) {
                   std::cerr << "Error: rect1 is invalid!" << std::endl;
                   return false;
               }

               if (rect2.w == 0 || rect2.h == 0) {
                   std::cerr << "Error: rect2 is invalid!" << std::endl;
                   return false;
               }

        return SDL_HasIntersection(&rect1, &rect2);
    }
};

#endif // COLLISION_H
