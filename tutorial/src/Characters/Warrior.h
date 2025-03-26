#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "SDL2/SDL.h"
#include "MapParser.h"
#include <memory>
#include "Checkpoint.h"
#include "Enemy.h"
#define ANIM_SPEED 80
#define JUMP_TIME 15.0f
#define JUMP_FORCE 10.0f

class Warrior : public Character {
public:
    Warrior(Properties* props, std::vector<MapParser*>& maps);
    virtual ~Warrior();

    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;
    void Update(float dt, std::vector<Enemy*>& enemies, int mapWidth, int mapHeight, std::vector<MapParser*> maps);


    // Getters for health and mana values
    float GetHealth() const { return m_Health; }
    float GetMaxHealth() const { return m_MaxHealth; }
    float GetMana() const { return m_Mana; }
    float GetMaxMana() const { return m_MaxMana; }

    // Functions to modify health and mana
    void TakeDamage(float damage);
    void UseMana(float amount);
    void Heal(float amount);
    // Set the size of the appearance effect (when spawning or respawning)
    void SetAppearEffectSize(int width, int height);

    void RestoreMana(float amount);
    void StartAppearEffect();
    // Set the last checkpoint where the warrior should respawn
    void SetLastCheckpoint(Checkpoint* checkpoint) { m_LastCheckpoint = checkpoint; }

    
    // Get the collision box of the warrior
    SDL_Rect GetCollider() const {
        return { static_cast<int>(m_Transform->X),
                 static_cast<int>(m_Transform->Y),
                 m_Width,
                 m_Height };
    }

private:
    void HandleRespawn(float dt);
    void Respawn();
    
    std::unique_ptr<Animation> m_Animation;
    std::unique_ptr<RigidBody> m_RigidBody;
    std::unique_ptr<Animation> m_AppearEffect;
    bool m_IsAppearing = false;
    float m_AppearTimer = 0.0f;
    Checkpoint* m_LastCheckpoint;
    std::vector<MapParser*> m_Maps;
    SDL_Rect m_BoundingBox;
    float m_Health;
    float m_MaxHealth;
    float m_Mana;
    float m_MaxMana;
    bool m_IsJumping;
    bool m_IsGrounded;
    bool m_IsDead = false;
    bool m_IsRight = true;
    float m_RespawnCounter;
    float m_JumpTime;
    float m_JumpForce;
    float m_RespawnX = 100.0f;
    float m_RespawnY = 200.0f;
    int m_AppearWidth = 96;
    int m_AppearHeight = 96;
    SDL_RendererFlip Player_flip = SDL_FLIP_NONE;
    

};

#endif // WARRIOR_H
