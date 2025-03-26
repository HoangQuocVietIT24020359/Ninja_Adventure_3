#include "Camera.h"
#include "Engine.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt) {
    if (m_Target != nullptr) {
        // Move the camera to center the target
        // static_cast<int>(...) convert to int 
        m_ViewBox.x = static_cast<int>(m_Target->X - (SCREEN_WIDTH / 2));
        m_ViewBox.y = static_cast<int>(m_Target->Y - (SCREEN_HEIGHT / 2));

        // Ensure the camera does not move out of the map boundaries
        if (m_ViewBox.x < 0) m_ViewBox.x = 0;
        if (m_ViewBox.y < 0) m_ViewBox.y = 0;
        if (m_ViewBox.x > m_MapWidth - m_ViewBox.w) m_ViewBox.x = m_MapWidth - m_ViewBox.w;
        if (m_ViewBox.y > m_MapHeight - m_ViewBox.h) m_ViewBox.y = m_MapHeight - m_ViewBox.h;
        
        // Update the camera's position vector
        m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y);
    }
}
