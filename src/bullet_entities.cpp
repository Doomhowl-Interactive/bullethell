#include <cstring>
#include <string>

#include "bullet_entities.hpp"
#include "bullet_scenes.hpp"

Entity::Entity(size_t id, Scene* scene)
{
    this->id = id;
    this->scene = scene;
    this->isActive = true;
    this->frameInterval = 0.2f;
    this->tint = WHITE;
}

void Entity::Reset()
{
    memset(this, 0, sizeof(Entity));
}

void Entity::SetCenter(float x, float y)
{
    this->bounds.x = x - this->bounds.width * 0.5f;
    this->bounds.y = y - this->bounds.height * 0.5f;
}

void Entity::SetCenter(Vector2 center)
{
    SetCenter(center.x, center.y);
}

Vector2 Entity::GetCenter() const
{
    Vector2 center = {
        this->bounds.x + this->bounds.width * 0.5f,
        this->bounds.x + this->bounds.width * 0.5f,
    };
    return center;
}

void Entity::SetSize(uint width, uint height)
{
    Vector2 center = this->GetCenter();
    this->bounds.x = center.x - (float)width * 0.5f;
    this->bounds.y = center.y - (float)height * 0.5f;
    this->bounds.width = (float)width;
    this->bounds.height = (float)height;
}

void Entity::ResetVelocity()
{
    this->vel.x = 0;
    this->vel.y = 0;
}

void Entity::Update(float delta)
{
    this->timeAlive += delta;

    // Entity drawing
    if (timer > frameInterval) {
        timer = 0.f;
        frameID++;
    }
    timer += delta;

    if (e->texture.width > 0) {
        if (e->texture.pixels) {
            Rectangle sourceRec = { sourceOffset.x + bounds.x, sourceOffset.y + bounds.y, bounds.width, bounds.height };
            DrawTexturePro(texture, sourceRec, bounds, Vector2Zero(), 0.f, tint);
        } else {
            DrawRectangleRec(bounds, tint);
        }
    }

    // WEAPON BEHAVIOUR
    for (uint i = 0; i < MAX_SPAWNERS; i++) {
        BulletSpawner* weapon = &bulletSpawners[i];
        if (weapon->patternToSpawn == nullptr || weapon->disabled)
            continue;

        // draw normal (debugging)
        Vector2 center = GetCenter();
        Vector2 weaponCenter = Vec2Add(center, weapon->offset);
        Vector2 end = Vec2Add(weaponCenter, Vec2Scale(weapon->normal, 10.f));
        DrawLineV(weaponCenter, end, BLUE);

        // spawn bullets on interval
        if (weapon->interval > 0.f && weapon->spawnTimer > weapon->interval) {
            Entity* bul = CreateEntity(scene);
            InitBullet(bul, weapon->patternToSpawn, weaponCenter, weapon->normal);
            weapon->spawnTimer = 0.f;
        }
        weapon->spawnTimer += delta;
    }

    // Bullet behaviour
    if (RunBulletPattern(this, delta)) {
        Reset();
        return;
    }

    // Ship ai behaviour
    RunEntityAI(e, delta);

    // apply movement
    e->bounds.x += vel->x * delta;
    e->bounds.y += vel->y * delta;
}

void Entity::CheckCollision(const Entity* other)
{
    if (this->id == other->id) {
        return;
    }

    if (other->HasFlag(this->ignoreFlags)) {
        return;
    }

    if (CheckCollisionRecs(this->bounds, other->bounds)) {
        OnCollision(other);
    }
}

bool Entity::HasFlag(EntityFlag flag) const
{
    if (flag == 0) {
        return false;
    }
    return ((flags & flag) == flag);
}

void Entity::Damage(int amount)
{
    health -= amount;
    if (health <= 0) {
        Reset();
    }
}

void Entity::OnCollision(const Entity* other)
{
    // TODO: implement
}
