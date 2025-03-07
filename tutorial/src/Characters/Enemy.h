#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "MapParser.h"
#include <memory>

class Enemy : public Character {
public:
    Enemy(Properties* props, MapParser* mapParser, int walkFrames); // Thêm đầy đủ tham số
    virtual ~Enemy();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override; // Ghi đè đúng cách
    void Update(float dt, MapParser* MapParsers);


    // Đánh dấu kẻ địch là đã chết
    void SetDead(bool dead);
    
    // Kiểm tra kẻ địch có chết hay không
    bool IsDead() const { return m_IsDead; }

    // Getter cho Collider
    SDL_Rect GetCollider() const {
        return { static_cast<int>(m_Transform->X),
                 static_cast<int>(m_Transform->Y),
                 m_Width,
                 m_Height };
    }

private:
    std::unique_ptr<Animation> m_Animation;
    std::unique_ptr<RigidBody> m_RigidBody;
    
    MapParser* m_MapParser;
    bool movingRight;
    float moveSpeed;
    float leftBoundary;
    float rightBoundary;
    std::string m_TextureID;
    int m_WalkFrames;  // Số frame khi đi
    int m_DeadFrames;  // Số frame khi chết


    bool m_IsDead = false; // Thêm biến để kiểm tra trạng thái kẻ địch
};

#endif // ENEMY_H
