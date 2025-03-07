#include "Enemy.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Collision.h"


Enemy::Enemy(Properties* props, MapParser* mapParser, int walkFrames)
    : Character(props), m_MapParser(mapParser), movingRight(true), moveSpeed(2.0f),
      m_WalkFrames(walkFrames) { // Nhận số frame

    m_TextureID = props->TextureID; // Lưu TextureID riêng cho từng enemy

    m_Animation = std::make_unique<Animation>();
    m_RigidBody = std::make_unique<RigidBody>();
    m_Animation->SetProps(m_TextureID, 1, m_WalkFrames, 100); // Dùng số frame riêng

    leftBoundary = m_Transform->X - 200;
    rightBoundary = m_Transform->X + 200;
}



Enemy::~Enemy() {
    Clean();
}

void Enemy::Update(float dt) {
    // Triển khai logic cơ bản để tránh lỗi lớp trừu tượng
}

void Enemy::Update(float dt,  MapParser* MapParser) {
    m_MapParser = MapParser;
    m_Animation->SetProps(m_TextureID, 1, 8, 100);
    m_RigidBody->UnSetForce();

    // Quái vật di chuyển trái/phải trong phạm vi giới hạn
    if (movingRight) {
        m_RigidBody->ApplyForceX(moveSpeed);
        if (m_Transform->X >= rightBoundary) {
            movingRight = false;
        }
    } else {
        m_RigidBody->ApplyForceX(-moveSpeed);
        if (m_Transform->X <= leftBoundary) {
            movingRight = true;
        }
    }
    if (m_TextureID == "snail"){
        if (movingRight){
            m_Animation->SetProps(m_TextureID, 1, m_WalkFrames, 100, SDL_FLIP_HORIZONTAL);
        }
    }
    else{
        if (!movingRight) {
            m_Animation->SetProps(m_TextureID, 1, m_WalkFrames, 100, SDL_FLIP_HORIZONTAL);
        }
    }
    

    bool collisionY = false;

    // Kiểm tra va chạm với solidTiles
    if (m_MapParser && !m_MapParser->solidTiles.empty()) {
        SDL_Rect nextPositionX = {
            static_cast<int>(m_Transform->X + m_RigidBody->NextPositionX(dt)),
            static_cast<int>(m_Transform->Y),
            m_Width,
            m_Height
        };

        for (const auto& tile : m_MapParser->solidTiles) {
            if (Collision::CheckCollision(nextPositionX, tile)) {
                movingRight = !movingRight;  // Đổi hướng nếu va chạm
                break;
            }
        }

        SDL_Rect nextPositionY = {
            static_cast<int>(m_Transform->X),
            static_cast<int>(m_Transform->Y + m_RigidBody->NextPositionY(dt)),
            m_Width,
            m_Height
        };

        for (const auto& tile : m_MapParser->solidTiles) {
            if (Collision::CheckCollision(nextPositionY, tile)) {
                collisionY = true;
                m_RigidBody->ApplyForceY(0);
                m_Transform->Y = tile.y - m_Height - 1;
                break;
            }
        }
    }

    // Cập nhật vị trí
    m_RigidBody->Update(dt, collisionY);
    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);

    // Cập nhật điểm gốc
    m_Origin->X = m_Transform->X + m_Width / 2;
    m_Origin->Y = m_Transform->Y + m_Height / 2;

    m_Animation->Update();
}


void Enemy::Draw() {
    m_Animation->Draw(m_Transform->X - Camera::GetInstance()->GetPosition().X,
                      m_Transform->Y - Camera::GetInstance()->GetPosition().Y,
                      m_Width, m_Height);
    
    
//    // Vẽ bounding box
//      SDL_Rect boundingBox = {
//          static_cast<int>(m_Transform->X - Camera::GetInstance()->GetPosition().X),
//          static_cast<int>(m_Transform->Y - Camera::GetInstance()->GetPosition().Y),
//          m_Width,
//          m_Height
//      };
//
//      // Sử dụng Engine để lấy renderer
//      SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 255, 0, 0, 255); // Màu đỏ
//      SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &boundingBox);
}

void Enemy::Clean() {
    m_Animation.reset();
    m_RigidBody.reset();
}

void Enemy::SetDead(bool dead) {
    m_IsDead = dead;
}
