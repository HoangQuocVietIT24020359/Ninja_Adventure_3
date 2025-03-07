#ifndef CAMERA_H
#define CAMERA_H

#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "Point.h"
#include "Engine.h"
#include "MapParser.h"

class Camera {
public:
    void Update(float dt);
    inline SDL_Rect GetViewBox() { return m_ViewBox; }
    inline Vector2D GetPosition() { return m_Position; }
    inline void SetTarget(Point* target) { m_Target = target; }
    inline static Camera* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Camera(); }
    
    void SetMapSize(int width, int height) { m_MapWidth = width; m_MapHeight = height; }

private:
    Camera() : m_MapWidth(0), m_MapHeight(0) {
        m_ViewBox = {0, 0, 1280, 720};
    }

    Point* m_Target;
    Vector2D m_Position;
    
    SDL_Rect m_ViewBox;
    int m_MapWidth, m_MapHeight;
    static Camera* s_Instance;
};

#endif // CAMERA_H
