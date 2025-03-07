#include "Warrior.h"
#include "SDL2/SDL.h"
#include "TextureManager.h"
#include "Input.h"
#include "Camera.h"
#include "MapParser.h"
#include "Collision.h"
#include "iostream"
#include "SoundManager.h"
using namespace std;



Warrior::Warrior(Properties* props, std::vector<MapParser*>& maps)
    : Character(props), m_Maps(maps), m_IsJumping(false) {  // Thêm biến m_IsJumping
    
    
    m_LastCheckpoint = nullptr;
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
        
    m_Animation = std::make_unique<Animation>();
    m_RigidBody = std::make_unique<RigidBody>();
    m_Animation->SetProps(m_TextureID, 1, 6, 100);
        
    m_AppearEffect = std::make_unique<Animation>();
    m_AppearEffect->SetProps("appear", 1, 15, 100);
    // Khởi tạo giá trị máu và mana
    m_MaxHealth = 50.0f;
    m_Health = m_MaxHealth;
    m_MaxMana = 50.0f;
    m_Mana = m_MaxMana;
        
        
    
    // Vị trí hồi sinh mặc định
    m_RespawnX = 100;
    m_RespawnY = 200;
}

Warrior::~Warrior() {
    Clean();
}

void Warrior::Update(float dt) {
    // Triển khai logic cơ bản để tránh lỗi lớp trừu tượng
}

void Warrior::Update(float dt, std::vector<Enemy*>& enemies, int mapWidth, int mapHeight,  std::vector<MapParser*> maps) {
    m_Maps = maps;
    if (m_IsAppearing) {
        m_AppearTimer -= dt;
        if (m_AppearTimer <= 0) {
            m_IsAppearing = false; // Tắt hiệu ứng sau 5 giây
        }
        m_AppearEffect->Update();
    }

    // Nếu nhân vật đang chết
      if (m_IsDead) {
          
          // Kiểm tra nếu animation chết đã chạy hết
          if (m_Animation->IsAnimationFinished()) {
              m_RespawnCounter -= dt;  // Giảm thời gian chờ hồi sinh
              
              // Khi thời gian chờ <= 0 thì hồi sinh
              if (m_RespawnCounter <= 0) {
                  m_IsRight = true;
                  Respawn();
              }
          }
          m_Animation->Update();
          return;
      }
    
    if (m_IsRight){
        m_Flip = SDL_FLIP_NONE;
    }
    else{
        m_Flip = SDL_FLIP_HORIZONTAL;
    }
    
    m_Animation->SetProps("player", 1, 6, 100, m_Flip);
    
    m_RigidBody->UnSetForce();

    // Kiểm tra di chuyển trái/phải
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)) {
        m_IsRight = false;
        m_RigidBody->ApplyForceX(5 * BACKWARD);
        m_Animation->SetProps("player_run", 1, 8, 100, SDL_FLIP_HORIZONTAL);
    } else if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D)) {
        m_IsRight = true;
        m_RigidBody->ApplyForceX(5 * FORWARD);
        m_Animation->SetProps("player_run", 1, 8, 100);
    }
    
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsGrounded) {
        m_IsJumping = true;
        m_IsGrounded = false;
        m_JumpTime = JUMP_TIME;  // Reset lại thời gian nhảy
        m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
    }
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsJumping && m_JumpTime > 0) {
        m_JumpTime -= dt;
        m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
       
    } else {
        m_IsJumping = false;
    }
    
    if (m_IsJumping || !m_IsGrounded){
        if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)){
            m_Animation->SetProps("player_jump", 1, 7, 150, SDL_FLIP_HORIZONTAL);
        }
        else{
            m_Animation->SetProps("player_jump", 1, 7, 150);
        }
        
    }
    
    
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
        m_Animation->SetProps("player_attack", 1, 5, 150, m_Flip);
        SoundManager::GetInstance()->PlaySound("attack");
            for (auto it = enemies.begin(); it != enemies.end();) {
                Enemy* enemy = *it;
                if (Collision::CheckCollision(this->GetCollider(), enemy->GetCollider())) {
                    SoundManager::GetInstance()->PlaySound("enemy_hit");
                    enemy->SetDead(true);
                    delete enemy;  // Giải phóng bộ nhớ
                    it = enemies.erase(it);  // Xóa khỏi vector
                    
                } else {
                    ++it;
                   
                }
            }
        }

    
    bool collisionY = false;

    // Kiểm tra va chạm
    if (!m_Maps.empty()) {
        // Kiểm tra va chạm theo trục X
        // Điều chỉnh bounding box nhỏ hơn một chút để tránh va chạm sai
        int boxOffsetX = 35; // Giảm chiều rộng 2 bên
        int boxOffsetY = 0; // Giảm chiều cao trên dưới
        int boxWidth = m_Width - (2 * boxOffsetX);
        int boxHeight = m_Height - (2 * boxOffsetY);

        SDL_Rect nextPositionX = {
            static_cast<int>(m_Transform->X + m_RigidBody->NextPositionX(dt)) + boxOffsetX,
            static_cast<int>(m_Transform->Y) + boxOffsetY,
            boxWidth,
            boxHeight
        };

        SDL_Rect nextPositionY = {
            static_cast<int>(m_Transform->X) + boxOffsetX,
            static_cast<int>(m_Transform->Y + m_RigidBody->NextPositionY(dt)) + boxOffsetY,
            boxWidth,
            boxHeight
        };
        
        for (auto& map : m_Maps) {
            if (map && !map->solidTiles.empty()) {
                for (const auto& tile : map->solidTiles) {
                    if (Collision::CheckCollision(nextPositionX, tile)) {
                        m_RigidBody->ApplyForceX(0);
                        break;
                    }
                }
            }
        }
        
        if (!m_IsJumping) {
            for (auto& map : m_Maps) {
                for (const auto& tile : map->solidTiles) {
                    if (Collision::CheckCollision(nextPositionY, tile)) {
                        collisionY = true;
                        m_RigidBody->SetVelocityY(0);
                        m_Transform->Y = tile.y - m_Height - 1;
                        m_IsGrounded = true;
                        break;
                    }
                }
            }
        }
    }

    m_RigidBody->Update(dt, collisionY);
    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);

    // Giới hạn di chuyển trong màn hình
    if (m_Transform->X < 0) m_Transform->X = 0;
    if (m_Transform->X + m_Width > mapWidth) m_Transform->X = mapWidth - m_Width;
    
    if (m_Transform->Y > mapHeight) {
        TakeDamage(m_Health);
    }
    m_Origin->X = m_Transform->X + m_Width / 2;
    m_Origin->Y = m_Transform->Y + m_Height / 2;

    m_Animation->Update();
}

void Warrior::TakeDamage(float damage) {
    m_Health -= damage;
    if (m_Health <= 0 && !m_IsDead) {
        m_Health = 0;
        m_IsDead = true;
//        SoundManager::GetInstance()->PlaySound("player_hit");
        m_RespawnCounter = 15.0f; // Thời gian chờ hồi sinh (3 giây)
        m_Animation->SetProps("player_dead", 1, 6, 400, m_Flip);  // Chạy animation chết
      
    }
    if (m_Health > 0){
        SoundManager::GetInstance()->PlaySound("player_hit");
    }
}

void Warrior::HandleRespawn(float dt) {
    if (m_Animation->IsAnimationFinished()) {
        m_RespawnCounter -= dt;
        if (m_RespawnCounter <= 0) {
            Respawn();
        }
    }
    m_Animation->Update();
}

void Warrior::Respawn() {
    m_IsDead = false;
    m_IsRight = true;
    m_Health = m_MaxHealth;
    m_Animation->SetProps("player", 1, 6, 100);
    
 
    // Nếu có checkpoint cuối cùng, hồi sinh tại đó
    if (m_LastCheckpoint) {
        m_Transform->X = m_LastCheckpoint->GetX();
        m_Transform->Y = m_LastCheckpoint->GetY();
    } else {
        // Nếu chưa có checkpoint nào, hồi sinh ở vị trí mặc định
        m_Transform->X = m_RespawnX;
        m_Transform->Y = m_RespawnY;
    }
    StartAppearEffect(); // Kích hoạt hiệu ứng xuất hiện
    SoundManager::GetInstance()->PlaySound("buff");
}

void Warrior::StartAppearEffect() {
    m_IsAppearing = true;
    m_AppearTimer = 60.0f; // Hiệu ứng tồn tại trong 5 giây
    m_AppearEffect->SetProps("appear", 1, 15, 100); // 6 frames, tốc độ 100ms mỗi frame
}

void Warrior::SetAppearEffectSize(int width, int height) {
    m_AppearWidth = width;
    m_AppearHeight = height;
}



void Warrior::UseMana(float amount) {
    if (m_Mana >= amount) {
        m_Mana -= amount;
    }
}

void Warrior::Heal(float amount) {
    m_Health += amount;
    if (m_Health > m_MaxHealth) m_Health = m_MaxHealth;
    StartAppearEffect();
    SoundManager::GetInstance()->PlaySound("buff");
}

void Warrior::RestoreMana(float amount) {
    m_Mana += amount;
    if (m_Mana > m_MaxMana) m_Mana = m_MaxMana;
    StartAppearEffect();
    SoundManager::GetInstance()->PlaySound("buff");
}

void Warrior::Draw() {
    m_Animation->Draw(m_Transform->X - Camera::GetInstance()->GetPosition().X,
                      m_Transform->Y - Camera::GetInstance()->GetPosition().Y,
                      m_Width, m_Height);
    
    if (m_IsAppearing) {
        m_AppearEffect->Draw(m_Transform->X - Camera::GetInstance()->GetPosition().X - 20,
                              m_Transform->Y - Camera::GetInstance()->GetPosition().Y + 50,
                              m_AppearWidth, m_AppearHeight);
    }
    
//    // Điều chỉnh bounding box nhỏ hơn một chút
//        int boxOffsetX = 35; // Giảm chiều rộng 2 bên
//        int boxOffsetY = 0; // Giảm chiều cao trên dưới
//        int boxWidth = m_Width - (2 * boxOffsetX);
//        int boxHeight = m_Height - (2 * boxOffsetY);
//
//        SDL_Rect boundingBox = {
//            static_cast<int>(m_Transform->X - Camera::GetInstance()->GetPosition().X) + boxOffsetX,
//            static_cast<int>(m_Transform->Y - Camera::GetInstance()->GetPosition().Y) + boxOffsetY,
//            boxWidth,
//            boxHeight
//        };
//
//        // Sử dụng Engine để lấy renderer
//        SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 255, 0, 0, 255); // Màu đỏ
//        SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &boundingBox);
}

void Warrior::Clean() {
    m_Animation.reset();
    m_RigidBody.reset();
}
