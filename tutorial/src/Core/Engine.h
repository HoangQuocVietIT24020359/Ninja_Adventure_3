#ifndef ENGINE_H
#define ENGINE_H

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "MapParser.h"
#include "Camera.h"
#include "Collision.h" // Đảm bảo include nếu dùng va chạm
#include "Enemy.h" // Thêm Enemy
#include "Goal.h"
#include <vector>
#include "Menu.h" 

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Engine {
public:
    static Engine* GetInstance() {
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Engine();
    }

    bool Init();
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void Events();
    void LoadMapsForLevel(int level);
    void ResetGame();
    inline bool IsRunning() { return m_IsRunning; }
    inline SDL_Renderer* GetRenderer() { return m_Renderer; }

    // Trả về danh sách bản đồ
    std::vector<MapParser*>& GetMapLayers() { return maps; }

    // Trả về con trỏ bản đồ chính
    MapParser* GetMapParser() { return mapParser; }

    // Trả về kích thước màn hình
    int GetScreenWidth() const { return SCREEN_WIDTH; }
    int GetScreenHeight() const { return SCREEN_HEIGHT; }

private:
    Engine() {}
    
    bool m_IsRunning = false;
    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    SDL_Texture* m_Background = nullptr;

    std::vector<MapParser*> maps;  // Danh sách chứa nhiều bản đồ
    MapParser* mapParser = nullptr; // Con trỏ bản đồ chính
    
    Menu* menu = nullptr;  // Add Menu object
    std::vector<Enemy*> enemies; // Danh sách quái vật
    static Engine* s_Instance;
    Goal* goal;
    int currentLevel = 1;
    
    
};

#endif // ENGINE_H
