#include "HealthBar.h"
#include "string.h"

HealthBar::HealthBar(SDL_Renderer* renderer, int x, int y, int width, int height, const std::string& texturePath, BarType type)
    : m_Renderer(renderer), m_Value(1.0f), m_Type(type) {

    // Load texture
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load health bar texture: %s", SDL_GetError());
        m_Texture = nullptr;
    } else {
        m_Texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    // Thiết lập vị trí và kích thước thanh máu
    m_BarRect = { x, y, width, height };
    m_FillRect = { x + 2, y + 2, width - 4, height - 4 };

    // Vị trí texture (bên trái thanh máu)
    m_TextureRect = { x - 40, y - 15, 40, 40 };
}

HealthBar::~HealthBar() {
    if (m_Texture) {
        SDL_DestroyTexture(m_Texture);
    }
}

void HealthBar::Update(float value) {
    // Giới hạn giá trị trong khoảng [0, 1]
    m_Value = value;
    if (m_Value < 0.0f) m_Value = 0.0f;
    if (m_Value > 1.0f) m_Value = 1.0f;

    // Cập nhật chiều rộng của phần còn lại
    m_FillRect.w = static_cast<int>((m_BarRect.w - 4) * m_Value);
}

void HealthBar::Render() {
    // Vẽ texture nếu có
    if (m_Texture) {
        SDL_RenderCopy(m_Renderer, m_Texture, nullptr, &m_TextureRect);
    }

    // Viền thanh máu (màu đen)
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_Renderer, &m_BarRect);

    // Phần máu còn lại với màu thay đổi theo mức máu
    Uint8 r = 255, g = 0, b = 0;

    if (m_Type == BarType::HEALTH) {
        if (m_Value > 0.75f) {
            r = 255; g = 0; b = 0;
        } else if (m_Value > 0.5f) {
            r = 255; g = 50; b = 50;
        } else if (m_Value > 0.25f) {
            r = 255; g = 100; b = 100;
        } else {
            r = 255; g = 125; b = 125;
        }
    } else {
        r = 0; g = 0; b = 255;
    }

    SDL_SetRenderDrawColor(m_Renderer, r, g, b, 255);
    SDL_RenderFillRect(m_Renderer, &m_FillRect);

    // Phần đã mất (màu trắng)
    SDL_Rect lostHealthRect = {
        m_FillRect.x + m_FillRect.w,
        m_FillRect.y,
        (m_BarRect.w - 4) - m_FillRect.w,
        m_FillRect.h
    };
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(m_Renderer, &lostHealthRect);
}
