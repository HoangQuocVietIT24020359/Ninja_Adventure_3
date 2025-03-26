#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "MapParser.h"
#include <memory>

class Enemy : public Character {
public:
    Enemy(Properties* props, MapParser* mapParser, int walkFrames);
    // Destructor
    virtual ~Enemy();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;
    void Update(float dt, MapParser* MapParsers);

    void SetDead(bool dead);
    bool IsDead() const { return m_IsDead; }

    // Returns the enemy's collider as an SDL_Rect for collision detection
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
    float leftBoundary;   // Left boundary for enemy patrol
    float rightBoundary;  // Right boundary for enemy patrol
    std::string m_TextureID;
    int m_WalkFrames;
    int m_DeadFrames;


    bool m_IsDead = false; 
};

#endif // ENEMY_H
