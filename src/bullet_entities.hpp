#pragma once
#include "bullet_common.hpp"

typedef uint EntityFlag;
typedef size_t EntityID;

class UpdateAndDrawable {
   public:
    virtual void Draw() = 0;
    virtual void Update(float delta) = 0;
};

class Scene;
class Entity : public UpdateAndDrawable {
   public:
    Entity(EntityID id, Scene* scene);

    void Reset();
    void Update(float delta) override;
    void Draw() override;

    // entity mega struct
    Scene* scene;
    bool isActive;
    float timeAlive;
    EntityID id;
    EntityFlag flags;

    bool HasFlag(EntityFlag flag) const;

    // sprite
    Rectangle bounds;
    Color tint;
    Vector2 sourceOffset;
    Texture texture;

    Vector2 GetCenter() const;
    void SetCenter(float x, float y);
    inline void SetCenter(Vector2 center);

    void SetSize(uint width, uint height);

    float frameInterval;
    float timer;
    int frameID;

    // physics
    Vector2 vel;

    void ResetVelocity();

    // ship
    float moveSpeed;

    // health
    int maxHealth;
    int health;

    void Damage(int amount = 1);

    // collision
    bool isToucher;
    EntityFlag ignoreFlags;

    void CheckCollision(const Entity* other);

    // bullet
    BulletPattern bulletPattern;

    // spawner
    BulletSpawner bulletSpawners[MAX_SPAWNERS];

    // ai
    EntityAI ai;

   private:
    void OnCollision(const Entity* other);
};