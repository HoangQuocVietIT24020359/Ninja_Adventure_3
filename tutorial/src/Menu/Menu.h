#ifndef MENU_H
#define MENU_H

#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include <vector>
#include <string>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void Render(int level);
    int HandleEvent(SDL_Event& event);
    void Hide() { m_IsActive = false; m_IsWinActive = false; }
    void ShowMenu(){m_IsActive = true;}
    bool IsActive() { return m_IsActive; }
    bool IsWinActive() { return m_IsWinActive; }

    void ShowWinMenu(int level);  // Accepts the current level
    int HandleWinMenuEvent(SDL_Event& event); // Handles clicks on win menu buttons

private:
    SDL_Renderer* m_Renderer;
    TTF_Font* m_Font;
    bool m_IsActive;
    bool m_IsWinActive; // Biến kiểm tra menu chiến thắng có hiển thị hay không
  
    SDL_Texture* winTextTexture; // Texture để hiển thị chữ "You Win!"
    
    struct Button {
        SDL_Rect rect;
        SDL_Texture* textTexture;
        std::string text; // Thêm thuộc tính text để lưu nội dung chữ
        bool isHovered;
    };
    std::vector<Button> buttons;
    std::vector<Button> winButtons;  // Buttons for the win menu
    int currentLevel;
    void CreateLevelButtons();
    void CreateWinButtons();  // New function to create buttons for win menu
    void UpdateButtonTexture(Button& button, SDL_Color color);
    SDL_Texture* backgroundTexture;

};

#endif // MENU_H
