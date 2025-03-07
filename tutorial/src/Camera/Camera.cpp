#include "Camera.h"
#include "Engine.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt) {
    if (m_Target != nullptr) {
        // Dịch camera theo mục tiêu
        m_ViewBox.x = static_cast<int>(m_Target->X - (SCREEN_WIDTH / 2));
        m_ViewBox.y = static_cast<int>(m_Target->Y - (SCREEN_HEIGHT / 2));

        // Giới hạn phạm vi camera trong bản đồ
        if (m_ViewBox.x < 0) m_ViewBox.x = 0;
        if (m_ViewBox.y < 0) m_ViewBox.y = 0;
        if (m_ViewBox.x > m_MapWidth - m_ViewBox.w) m_ViewBox.x = m_MapWidth - m_ViewBox.w;
        if (m_ViewBox.y > m_MapHeight - m_ViewBox.h) m_ViewBox.y = m_MapHeight - m_ViewBox.h;
        
        // Cập nhật vị trí camera
        m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y);
    }
}
