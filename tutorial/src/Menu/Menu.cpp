#include "Menu.h"
#include <iostream>
#include "SDL2_image/SDL_image.h"

Menu::Menu(SDL_Renderer* renderer) : m_Renderer(renderer), m_IsActive(true), m_IsWinActive(false) {
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        exit(1);
    }

    m_Font = TTF_OpenFont("assets/Fonts/Roboto_Regular.ttf", 24);
    if (!m_Font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        exit(1);
    }
    
    SDL_Surface* bgSurface = IMG_Load("assets/background/bg_menu.png");
    if (!bgSurface) {
        std::cerr << "Failed to load background: " << IMG_GetError() << std::endl;
        exit(1);
    }
    backgroundTexture = SDL_CreateTextureFromSurface(m_Renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    CreateLevelButtons();

    // Tạo texture chữ "You Win!"
    SDL_Surface* winSurface = TTF_RenderText_Solid(m_Font, "YOU WIN!", {255, 0, 0});
    SDL_Surface* lossSurface = TTF_RenderText_Solid(m_Font, "GAME OVER!", {255, 0, 0});
    winTextTexture = SDL_CreateTextureFromSurface(m_Renderer, winSurface);
    lossTextTexture = SDL_CreateTextureFromSurface(m_Renderer, lossSurface);
    SDL_FreeSurface(winSurface);
    SDL_FreeSurface(lossSurface);
}

void Menu::ShowWinMenu(int level) {
    m_IsWinActive = true;
    m_IsActive = false;
    m_IsLossActive = false;
    currentLevel = level;
    CreateWinButtons();
}

void Menu::ShowLossMenu(int level) {
    m_IsWinActive = false;
    m_IsActive = false;
    m_IsLossActive = true;
    currentLevel = level;
    CreateLossButtons();
}


Menu::~Menu() {
    for (auto& button : buttons) {
        SDL_DestroyTexture(button.textTexture);
    }
    TTF_CloseFont(m_Font);
}

void Menu::CreateLevelButtons() {
    int startX = 250, startY = 200;
    int buttonWidth = 120, buttonHeight = 120;
    int columns = 5;
    
    for (int i = 1; i <= 10; i++) {
        Button btn;
        btn.rect = { startX + ((i - 1) % columns) * (buttonWidth + 30),
                     startY + ((i - 1) / columns) * (buttonHeight + 30),
                     buttonWidth, buttonHeight };
        btn.text = std::to_string(i);
        btn.isHovered = false;
        UpdateButtonTexture(btn, {204, 204, 204});
        buttons.push_back(btn);
    }
}

void Menu::UpdateButtonTexture(Button& button, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(m_Font, button.text.c_str(), {255, 255, 255});
    button.textTexture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    SDL_FreeSurface(surface);
}


void Menu::Render(int level) {
    SDL_RenderCopy(m_Renderer, backgroundTexture, NULL, NULL);
    // Render level selection menu if active
    if (m_IsActive) {
        for (auto& button : buttons) {
            SDL_SetRenderDrawColor(m_Renderer,
                button.isHovered ? 0 : 204,
                button.isHovered ? 255 : 204,
                button.isHovered ? 204 : 204,
                255);
            SDL_RenderFillRect(m_Renderer, &button.rect);
            SDL_RenderCopy(m_Renderer, button.textTexture, NULL, &button.rect);
        }
    }
    else if (m_IsWinActive) {
        // Render "You Win!" text
        SDL_Rect winRect = {440, 100, 400, 200};
        SDL_RenderCopy(m_Renderer, winTextTexture, NULL, &winRect);

        // Render win menu buttons
        for (auto& button : winButtons) {
            if (level == 10 and button.text == "Next Level"){
                continue;
            }
            SDL_SetRenderDrawColor(m_Renderer,
                button.isHovered ? 0 : 204,
                button.isHovered ? 255 : 204,
                button.isHovered ? 204 : 204,
                255);
            SDL_RenderFillRect(m_Renderer, &button.rect);
            SDL_RenderCopy(m_Renderer, button.textTexture, NULL, &button.rect);
        }
    }
    
    else if (m_IsLossActive) {
            SDL_Rect lossRect = {440, 100, 400, 200};
            SDL_RenderCopy(m_Renderer, lossTextTexture, NULL, &lossRect);

            for (auto& button : lossButtons) {
                SDL_SetRenderDrawColor(m_Renderer,
                    button.isHovered ? 0 : 204,
                    button.isHovered ? 255 : 204,  
                    button.isHovered ? 204 : 204,
                    255);
                SDL_RenderFillRect(m_Renderer, &button.rect);
                SDL_RenderCopy(m_Renderer, button.textTexture, NULL, &button.rect);
            }
        }
}

int Menu::HandleEvent(SDL_Event& event) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    for (auto& button : buttons) {
        bool isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                          y >= button.rect.y && y <= button.rect.y + button.rect.h);

        button.isHovered = isHovered;

        if (event.type == SDL_MOUSEBUTTONDOWN && isHovered) {
            Hide();
            return std::stoi(button.text);
        }
    }
    
    return -1;
}



int Menu::HandleWinMenuEvent(SDL_Event& event) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    for (size_t i = 0; i < winButtons.size(); i++) {
        auto& button = winButtons[i];
        bool isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                          y >= button.rect.y && y <= button.rect.y + button.rect.h);

        button.isHovered = isHovered;

        if (event.type == SDL_MOUSEBUTTONDOWN && isHovered) {
            return i + 1;
        }
    }

    return -1;
}


void Menu::CreateWinButtons() {
    winButtons.clear();

    int startX = 440, startY = 330;
    int buttonWidth = 400, buttonHeight = 60;
    
    std::vector<std::string> labels = {"Return to Menu", "Restart Level"};
    if (currentLevel < 20) {
        labels.push_back("Next Level");
    }

    for (int i = 0; i < labels.size(); i++) {
        Button btn;
        btn.rect = { startX, startY + i * (buttonHeight + 30), buttonWidth, buttonHeight };
        btn.text = labels[i];
        btn.isHovered = false;
        UpdateButtonTexture(btn, {0, 255, 0});  
        winButtons.push_back(btn);
    }
}

int Menu::HandleLossMenuEvent(SDL_Event& event) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    for (size_t i = 0; i < lossButtons.size(); i++) {
        auto& button = lossButtons[i];
        bool isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                          y >= button.rect.y && y <= button.rect.y + button.rect.h);

        button.isHovered = isHovered;
        if (event.type == SDL_MOUSEBUTTONDOWN && isHovered) {
            return i + 1;
        }
    }

    return -1;
}


void Menu::CreateLossButtons() {
    lossButtons.clear();

    int startX = 440, startY = 330;
    int buttonWidth = 400, buttonHeight = 60;
    
    std::vector<std::string> labels = {"Return to Menu", "Restart Level"};
    
    for (int i = 0; i < labels.size(); i++) {
        Button btn;
        btn.rect = { startX, startY + i * (buttonHeight + 30), buttonWidth, buttonHeight };
        btn.text = labels[i];
        btn.isHovered = false;
        UpdateButtonTexture(btn, {0, 255, 0});
        lossButtons.push_back(btn);
    }
}
