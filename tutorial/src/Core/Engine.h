#ifndef ENGINE_H
#define ENGINE_H

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "MapParser.h"
#include "Camera.h"
#include "Collision.h"
#include "Enemy.h" 
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


    std::vector<MapParser*>& GetMapLayers() { return maps; }

    MapParser* GetMapParser() { return mapParser; }

    int GetScreenWidth() const { return SCREEN_WIDTH; }
    int GetScreenHeight() const { return SCREEN_HEIGHT; }

private:
    Engine() {}
    
    bool m_IsRunning = false;
    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    SDL_Texture* m_Background = nullptr;

    std::vector<MapParser*> maps;
    MapParser* mapParser = nullptr;
    
    Menu* menu = nullptr;  
    std::vector<Enemy*> enemies;
    static Engine* s_Instance;
    Goal* goal;
    int currentLevel = 1;
    
    
};

#endif // ENGINE_H
