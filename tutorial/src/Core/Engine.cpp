#include "Engine.h"
#include "TextureManager.h"
#include "vector2D.h"
#include "Warrior.h"
#include "Input.h"
#include "Timer.h"
#include "MapParser.h"
#include "Collision.h"
#include "Enemy.h"
#include "HealthBar.h"
#include "Checkpoint.h"
#include "Item.h"
#include "SoundManager.h"
#include "Menu.h"
#include "Goal.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

std::vector<Item*> items;
Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr;
HealthBar* healthBar;
HealthBar* manaBar;
std::vector<Checkpoint*> checkpoints;
Checkpoint* lastCheckpoint = nullptr;
std::vector<SDL_Texture*> backgrounds;
bool showWinMenu = false;
bool hasPlayedWinSound = false;

void ShowWinMenu(SDL_Renderer* renderer) {
    SDL_Rect menuRect = {200, 150, 400, 300};  // Position & size
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderFillRect(renderer, &menuRect);

    // Load a "YOU WIN" texture or render text using SDL_ttf
    // Example using SDL_ttf:
    TTF_Font* font = TTF_OpenFont("assets/Fonts/Roboto_Regular.ttf", 24);
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "YOU WIN!", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {490, 310, 300, 100};  // Position text inside menu
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_RenderPresent(renderer);  // Show menu

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}



bool  Engine::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow("Soft engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (m_Window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }
    
    if (m_Window == nullptr) {
        m_Window = SDL_CreateWindow("Soft engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (m_Window == nullptr) {
            SDL_Log("Failed to create Window: %s", SDL_GetError());
            return false;
        }
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }
    
    menu = new Menu(m_Renderer); // Initialize the menu
    
    TextureManager::GetInstance()->Load("background", "assets/background/BGFront.png");

    // Load multiple backgrounds
        std::vector<std::string> bgPaths = {
            "assets/background/CloudsBack.png",
            "assets/background/CloudsFront.png",
            "assets/background/BGBack.png",
            "assets/background/BGFront.png",
        };
        
    for (const auto& path : bgPaths) {
        TextureManager::GetInstance()->Load(path, path);
        backgrounds.push_back(TextureManager::GetInstance()->GetTexture(path));
    }
    
    if (!SoundManager::GetInstance()->Init()) {
        std::cerr << "Failed to initialize SoundManager\n";
        return false;
    }
    
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    SoundManager::GetInstance()->LoadMusic("background", "assets/sounds/background.mp3");
    SoundManager::GetInstance()->PlayMusic("background", -1);
    
    
       
    SoundManager::GetInstance()->LoadSoundEffect("attack", "assets/sounds/attackSound.mp3");
    SoundManager::GetInstance()->LoadSoundEffect("buff", "assets/sounds/buff.mp3");
    SoundManager::GetInstance()->LoadSoundEffect("enemy_hit", "assets/sounds/enemyhit.mp3");
    SoundManager::GetInstance()->LoadSoundEffect("level_finish", "assets/sounds/levelfinish.wav");
    SoundManager::GetInstance()->LoadSoundEffect("player_hit", "assets/sounds/playerhit.wav");
    
    TextureManager::GetInstance()->Load("player", "assets/characters/Samurai/Idle.png");
    TextureManager::GetInstance()->Load("player_run", "assets/characters/Samurai/Run.png");
    TextureManager::GetInstance()->Load("player_jump", "assets/characters/Samurai/Jump.png");
    TextureManager::GetInstance()->Load("player_attack", "assets/characters/Samurai/Attack_2.png");
    TextureManager::GetInstance()->Load("player_dead", "assets/characters/Samurai/Dead.png");
    TextureManager::GetInstance()->Load("player_hurt", "assets/characters/Samurai/Hurt.png");
    
    TextureManager::GetInstance()->Load("appear", "assets/characters/apearEffect.png");
    
    TextureManager::GetInstance()->Load("snail", "assets/enemy/snail.png");
    TextureManager::GetInstance()->Load("fox", "assets/enemy/fox.png");

    TextureManager::GetInstance()->Load("health_potion", "assets/Health/heath_potion.png");
    TextureManager::GetInstance()->Load("mana_potion", "assets/Health/mana_potion.png");
        
    items.push_back(new Item("health_potion", 1300, 375, 32, 32, ItemType::HEALTH));
    items.push_back(new Item("health_potion", 1800, 550, 32, 32, ItemType::HEALTH));
    items.push_back(new Item("health_potion", 2900, 550, 32, 32, ItemType::HEALTH));
//    items.push_back(new Item("mana_potion", 1000, 550, 32, 32, ItemType::MANA));

    // Create and load maps, ensuring solidTiles is correctly populated

    MapParser* map1 = new MapParser(m_Renderer);
    map1->LoadMap("assets/levels/level2/solid.map", "assets/maps/Ground.png", true, false);
    maps.push_back(map1);
    

    MapParser* map2 = new MapParser(m_Renderer);
    map2->LoadMap("assets/levels/level2/bridge.map", "assets/maps/Tileset.png", true, false);
    maps.push_back(map2);
    
    MapParser* map3 = new MapParser(m_Renderer);
    map3->LoadMap("assets/levels/level2/fence.map", "assets/maps/Tileset.png");
    maps.push_back(map3);

    
    MapParser* map4 = new MapParser(m_Renderer);
    map4->LoadMap("assets/levels/level2/trap.map", "assets/maps/Tileset.png", false, true);
    maps.push_back(map4);

    MapParser* map5 = new MapParser(m_Renderer);
    map5->LoadMap("assets/levels/level2/tree.map", "assets/maps/Trees.png");
    maps.push_back(map5);
    
    MapParser* map6 = new MapParser(m_Renderer);
    map6->LoadMap("assets/levels/level2/brush.map", "assets/maps/Ground.png");
    maps.push_back(map6);

    player = new Warrior(new Properties("player", 100, 200, 128, 128), maps);

    Camera::GetInstance()->SetTarget(player->GetOrigin());
    Camera::GetInstance()->SetMapSize(map1->GetMapWidth(), map1->GetMapHeight());

    
    Enemy* enemy1 = new Enemy(new Properties("snail", 1150, 311, 48, 32), map1, 8);
    enemies.push_back(enemy1);

    Enemy* enemy2 = new Enemy(new Properties("snail", 1350, 311, 48, 32), map1, 8);
    enemies.push_back(enemy2);
    
    Enemy* enemy3 = new Enemy(new Properties("snail", 2750, 500, 48, 32), map1, 8);
    enemies.push_back(enemy3);
    
    Enemy* enemy4 = new Enemy(new Properties("fox", 2600, 500, 80, 48), map1, 8);
    enemies.push_back(enemy4);
    
    Checkpoint* cp1 = new Checkpoint(m_Renderer, 1450, 300, 50, 100);
    checkpoints.push_back(cp1);
    
    goal = new Goal(m_Renderer, 3500, 445, 536, 466);

    healthBar = new HealthBar(m_Renderer, 50, 30, 200, 20, "assets/Health/heart.png", BarType::HEALTH);
//    manaBar = new HealthBar(m_Renderer, 50, 70, 200, 20, "assets/Health/mana.png", BarType::MANA);
    
    return m_IsRunning = true;
}

void Engine::Update() {
   
    
    float dt = Timer::GetInstance()->GetDeltaTime();
    player->Update(dt, enemies, maps[0]->GetMapWidth(), maps[0]->GetMapHeight(), maps);
    Camera::GetInstance()->Update(dt);

    static float damageCooldownEnemy = 0.0f; // Biến lưu thời gian chờ sát thương
    static float damageCooldownTrap = 0.0f; // Biến lưu thời gian chờ sát thương
    damageCooldownEnemy -= dt;
    damageCooldownTrap -= dt;

    // Kiểm tra va chạm giữa nhân vật và kẻ địch
    for (auto& enemy : enemies) {
        enemy->Update(dt, maps[0]);

        SDL_Rect playerBox = player->GetCollider();
        SDL_Rect enemyBox = enemy->GetCollider();

        if (Collision::CheckCollision(playerBox, enemyBox)) {
            if (damageCooldownEnemy <= 0.0f) {
                player->TakeDamage(10.0f); // Nhận 10 sát thương khi va chạm với enemy
                damageCooldownEnemy = 30.0f; // Chờ 3 giây trước khi nhận sát thương tiếp theo
            }
        }
    }

    // Kiểm tra va chạm giữa nhân vật và map4 (bẫy)
    if (damageCooldownTrap <= 0.0f) {
        SDL_Rect playerBox = player->GetCollider();

        MapParser* map4 = maps.size() > 3 ? maps[3] : nullptr; // Lấy map4 từ danh sách

        if (map4 && !map4->trapTiles.empty()) {
            for (auto& tile : map4->trapTiles) {
                if (Collision::CheckCollision(playerBox, tile)) {
                    player->TakeDamage(5.0f);
                    damageCooldownTrap = 30.0f;
                    break;
                }
            }
        }
    }
    
    // Kiểm tra va chạm với item
       for (auto it = items.begin(); it != items.end(); ) {
           if (Collision::CheckCollision(player->GetCollider(), (*it)->GetCollider())) {
               if ((*it)->GetType() == ItemType::HEALTH) {
                   player->Heal(20.0f);
               } else if ((*it)->GetType() == ItemType::MANA) {
                   player->RestoreMana(20.0f);
               }
               delete *it;
               it = items.erase(it);
           } else {
               ++it;
           }
       }
    
    
    for (auto& cp : checkpoints) {
        cp->Update(dt);
        if (Collision::CheckCollision(player->GetCollider(), cp->GetCollider()) && !cp->IsActivated()) {
            cp->Activate(); // Kích hoạt animation
            SoundManager::GetInstance()->PlaySound("buff");
            player->SetLastCheckpoint(cp); // Cập nhật checkpoint cuối cùng cho Warrior
            player->StartAppearEffect();
        }
    }


    

    float healthPercent = player->GetHealth() / player->GetMaxHealth();
//    float manaPercent = player->GetMana() / player->GetMaxMana();

    healthBar->Update(healthPercent);
//    manaBar->Update(manaPercent);
    

    if (Collision::CheckCollision(player->GetCollider(), goal->GetCollider())) {
        if (showWinMenu == false){
            menu->ShowWinMenu(currentLevel);
            if (!hasPlayedWinSound) {
                SoundManager::GetInstance()->PlaySound("level_finish");
                hasPlayedWinSound = true;  // Ensure sound plays only once
            }
            showWinMenu = true;
        }
       
    }

}


void Engine::Render() {
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_Renderer);
    
    if (menu->IsActive() || menu->IsWinActive()) {
        menu->Render(currentLevel);  // Render the menu if active
    }
    else{
        int offsetX = Camera::GetInstance()->GetPosition().X;
        int offsetY = Camera::GetInstance()->GetPosition().Y;
            
        for (size_t i = 0; i < backgrounds.size(); i++) {
            if (backgrounds[i]) {
                SDL_Rect srcRect = {0, 0, 512, 320}; // Kích thước gốc
                SDL_Rect dstRect = {0, 0, 1280, 720}; // Kích thước mới
                SDL_RenderCopy(m_Renderer, backgrounds[i], &srcRect, &dstRect);
            }
        }
        
        

        // Render tất cả các bản đồ
        for (auto& map : maps) {
            map->Render(offsetX, offsetY);
        }
        
        for (auto& cp : checkpoints) {
            cp->Render(offsetX, offsetY);
        }
        
        for (auto& enemy : enemies) {
            enemy->Draw();
        }
        
        for (auto& item : items) {
               item->Render();
           }
        
        goal->Render(offsetX, offsetY);

        
        healthBar->Render();
    //    manaBar->Render();



        // Render player
        player->Draw();
    }
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_IsRunning = false;
        }

        // Detect 'Q' key press to show/hide menu
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
            if (menu->IsActive()) {
                menu->Hide();
            } else {
                menu->ShowMenu();
            }
        }

        if (menu->IsActive()) {
            int selectedLevel = menu->HandleEvent(event);
            if (selectedLevel > 0) {
                LoadMapsForLevel(selectedLevel);
            }
        }

        if (menu->IsWinActive()) {
            int action = menu->HandleWinMenuEvent(event);
            if (action == 1) {
                menu->Hide();
                menu->ShowMenu();
            } else if (action == 2) {
                menu->Hide();
                LoadMapsForLevel(currentLevel);
            } else if (action == 3 && currentLevel < 10) {
                menu->Hide();
                LoadMapsForLevel(currentLevel + 1);
            }
        }
    }
}


void Engine::LoadMapsForLevel(int level) {
    currentLevel = level;

    // Giải phóng tất cả tài nguyên cũ trước khi load level mới
    ResetGame();

    // Load maps mới
    std::string levelPath = "assets/levels/level" + std::to_string(level) + "/";

    maps.push_back(new MapParser(m_Renderer));
    maps.back()->LoadMap((levelPath + "solid.map").c_str(), "assets/maps/Ground.png", true, false);

    maps.push_back(new MapParser(m_Renderer));
    maps.back()->LoadMap((levelPath + "bridge.map").c_str(), "assets/maps/Tileset.png", true, false);

    maps.push_back(new MapParser(m_Renderer));
    maps.back()->LoadMap((levelPath + "fence.map").c_str(), "assets/maps/Tileset.png");

    maps.push_back(new MapParser(m_Renderer));
    maps.back()->LoadMap((levelPath + "trap.map").c_str(), "assets/maps/Tileset.png", false, true);

    maps.push_back(new MapParser(m_Renderer));
    maps.back()->LoadMap((levelPath + "tree.map").c_str(), "assets/maps/Trees.png");

    maps.push_back(new MapParser(m_Renderer));
    maps.back()->LoadMap((levelPath + "brush.map").c_str(), "assets/maps/Ground.png");

    // Tạo lại player
    player = new Warrior(new Properties("player", 100, 200, 128, 128), maps);
    Camera::GetInstance()->SetTarget(player->GetOrigin());
    Camera::GetInstance()->SetMapSize(maps[0]->GetMapWidth(), maps[0]->GetMapHeight());


    // Thêm vật phẩm mới
    if (level != 2){
        // Thêm kẻ địch mới
        enemies.push_back(new Enemy(new Properties("snail", 1150, 311, 48, 32), maps[0], 8));
        enemies.push_back(new Enemy(new Properties("snail", 1350, 311, 48, 32), maps[0], 8));
        enemies.push_back(new Enemy(new Properties("snail", 2750, 500, 48, 32), maps[0], 8));
        enemies.push_back(new Enemy(new Properties("fox", 2600, 500, 80, 48), maps[0], 8));
        
        items.push_back(new Item("health_potion", 1300, 375, 32, 32, ItemType::HEALTH));
        // Thêm checkpoint
        checkpoints.push_back(new Checkpoint(m_Renderer, 1450, 300, 50, 100));
        
        items.push_back(new Item("health_potion", 1800, 550, 32, 32, ItemType::HEALTH));
        items.push_back(new Item("health_potion", 2900, 550, 32, 32, ItemType::HEALTH));
    }
    
    // Đặt lại mục tiêu (goal)
    goal = new Goal(m_Renderer, 3500, 445, 536, 466);
}


bool Engine::Clean() {
    TextureManager::GetInstance()->Clean();
    
    backgrounds.clear();
    // Xóa tất cả các bản đồ trong danh sách
    for (auto& map : maps) {
        delete map;
    }
    maps.clear();
    
    for (auto& enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    
    delete healthBar;
    delete manaBar;

    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    
    
    return false;
}

void Engine::Quit() {
    m_IsRunning = false;
    TTF_Quit();
}


void Engine::ResetGame() {
    // Xóa tất cả các enemy
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Xóa các vật phẩm (items)
    for (auto item : items) {
        delete item;
    }
    items.clear();

    // Xóa checkpoint
    for (auto cp : checkpoints) {
        delete cp;
    }
    checkpoints.clear();

    // Xóa maps
    for (auto map : maps) {
        delete map;
    }
    maps.clear();

    // Xóa player
    if (player) {
        delete player;
        player = nullptr;
    }

    // Xóa goal
    if (goal) {
        delete goal;
        goal = nullptr;
    }
    
    showWinMenu = false;
}
