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
    void Hide() { m_IsActive = false; m_IsWinActive = false;  m_IsLossActive = false;}
    void ShowMenu(){m_IsActive = true;}
    bool IsActive() { return m_IsActive; }
    bool IsWinActive() { return m_IsWinActive; }
    bool IsLossActive() { return m_IsLossActive; }

    void ShowWinMenu(int level);
    void ShowLossMenu(int level);
    int HandleWinMenuEvent(SDL_Event& event);
    int HandleLossMenuEvent(SDL_Event& event);

private:
    SDL_Renderer* m_Renderer;
    TTF_Font* m_Font;
    bool m_IsActive;
    bool m_IsWinActive;
    bool m_IsLossActive;
  
    SDL_Texture *winTextTexture, *lossTextTexture;

    
    struct Button {
        SDL_Rect rect;
        SDL_Texture* textTexture;
        std::string text;
        bool isHovered;
    };
    std::vector<Button> buttons;
    std::vector<Button> winButtons;
    std::vector<Button> lossButtons;
    int currentLevel;
    void CreateLevelButtons();
    void CreateWinButtons();
    void CreateLossButtons();
    void UpdateButtonTexture(Button& button, SDL_Color color);
    SDL_Texture* backgroundTexture;

};

#endif // MENU_H
