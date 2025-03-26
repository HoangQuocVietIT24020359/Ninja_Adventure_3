#include "Enemy.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Collision.h"


Enemy::Enemy(Properties* props, MapParser* mapParser, int walkFrames)
    : Character(props), m_MapParser(mapParser), movingRight(true), moveSpeed(2.0f),
      m_WalkFrames(walkFrames) {

    m_TextureID = props->TextureID; // Store unique texture ID for each enemy

    m_Animation = std::make_unique<Animation>();
    m_RigidBody = std::make_unique<RigidBody>();
    m_Animation->SetProps(m_TextureID, 1, m_WalkFrames, 100); // Dùng số frame riêng
          
    // Define the patrol boundaries (enemy moves within ±200 pixels from spawn point)
    leftBoundary = m_Transform->X - 200;
    rightBoundary = m_Transform->X + 200;
}



Enemy::~Enemy() {
    Clean();
}

void Enemy::Update(float dt) {
    // Placeholder for inherited Update function
}

void Enemy::Update(float dt,  MapParser* MapParser) {
    m_MapParser = MapParser;  // Update the map reference
    m_Animation->SetProps(m_TextureID, 1, 8, 100);
    m_RigidBody->UnSetForce();

    // Enemy movement logic (patrols left and right)
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
    
    // Handle sprite flipping for animation
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

    // Collision detection with solid tiles
    if (m_MapParser && !m_MapParser->solidTiles.empty()) {
        // Predict next horizontal position for collision check
        SDL_Rect nextPositionX = {
            static_cast<int>(m_Transform->X + m_RigidBody->NextPositionX(dt)),
            static_cast<int>(m_Transform->Y),
            m_Width,
            m_Height
        };
        
        // Check for horizontal collisions
        for (const auto& tile : m_MapParser->solidTiles) {
            if (Collision::CheckCollision(nextPositionX, tile)) {
                movingRight = !movingRight;  // Reverse direction on collision
                break;
            }
        }
        // Predict next vertical position for collision check
        SDL_Rect nextPositionY = {
            static_cast<int>(m_Transform->X),
            static_cast<int>(m_Transform->Y + m_RigidBody->NextPositionY(dt)),
            m_Width,
            m_Height
        };
        
        // Check for vertical collisions
        for (const auto& tile : m_MapParser->solidTiles) {
            if (Collision::CheckCollision(nextPositionY, tile)) {
                collisionY = true;
                m_RigidBody->ApplyForceY(0); // Stop downward force
                m_Transform->Y = tile.y - m_Height - 1; // Adjust position to stand on tile
                break;
            }
        }
    }

    // Apply physics updates
    m_RigidBody->Update(dt, collisionY);
    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);

    // Update enemy's origin point (used for rotations or effects)
    m_Origin->X = m_Transform->X + m_Width / 2;
    m_Origin->Y = m_Transform->Y + m_Height / 2;

    m_Animation->Update();
}


void Enemy::Draw() {
    m_Animation->Draw(m_Transform->X - Camera::GetInstance()->GetPosition().X,
                      m_Transform->Y - Camera::GetInstance()->GetPosition().Y,
                      m_Width, m_Height);
    
    
    // Debug: Draw bounding box around the enemy (optional)
       /*
       SDL_Rect boundingBox = {
           static_cast<int>(m_Transform->X - Camera::GetInstance()->GetPosition().X),
           static_cast<int>(m_Transform->Y - Camera::GetInstance()->GetPosition().Y),
           m_Width,
           m_Height
       };

       // Render bounding box in red
       SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 255, 0, 0, 255);
       SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &boundingBox);
       */
}

void Enemy::Clean() {
    m_Animation.reset();
    m_RigidBody.reset();
}

void Enemy::SetDead(bool dead) {
    m_IsDead = dead;
}
