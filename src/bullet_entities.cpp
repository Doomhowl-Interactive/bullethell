#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.hpp"

#include <stdlib.h>
#include <string.h>

using namespace std;

BULLET void ClearEntities(Scene* scene)
{
    assert(scene);
    scene->entities.clear();
    INFO("Cleared all entities!");
}

// TODO: constructor
static void InitEntity(Entity* entity, Scene* scene)
{
    static usize nextID = 0;
    entity->id = nextID++;
    entity->scene = scene;
}

// TODO: Deprecate and use unordered_map
BULLET Entity* CreateEntity(Scene* scene)
{
    assert(scene);

    Entity entity = {};
    InitEntity(&entity, scene);
    scene->entities.insert({ entity.id, entity });
    return &scene->entities[entity.id];
}

BULLET void DestroyEntity(Entity* e)
{
    memset(e, 0, sizeof(Entity));
}

BULLET void SetEntityCenter(Entity* e, float x, float y)
{
    e->bounds.x = x - e->bounds.width * 0.5f;
    e->bounds.y = y - e->bounds.height * 0.5f;
}

BULLET Vec2 GetEntityCenter(Entity* e)
{
    return RectFCenter(e->bounds);
}

// TODO: When size is zero automatically use texture size
BULLET void SetEntitySize(Entity* e, uint width, uint height)
{
    Vec2 center = GetEntityCenter(e);
    e->bounds.x = center.x - width * 0.5f;
    e->bounds.y = center.y - height * 0.5f;
    e->bounds.width = width;
    e->bounds.height = height;
}

BULLET void ResetEntityVelocity(Entity* e)
{
    e->vel.x = 0;
    e->vel.y = 0;
}

BULLET void UpdateAndRenderEntity(Scene* scene, Texture canvas, Entity* e, float delta)
{
    e->timeAlive += delta;

    // Shortcuts
    Vec2 const* vel = &e->vel;

    // Sprite drawing
    auto& sprite = e->sprite;
    if (sprite.timer > sprite.frameInterval) {
        sprite.timer = 0.f;
        sprite.frameIndex++;
    }
    sprite.timer += delta;

    if (sprite.texture.width > 0) {
        if (sprite.texture.pixels) {
            DrawTextureEx(canvas,
                          sprite.texture,
                          V2(e->bounds),
                          0,
                          0,
                          e->bounds.width,
                          e->bounds.height,
                          e->tint);
        } else {
            DrawRectangle(canvas, R2(e->bounds), e->tint);
        }
    }

    Vec2 center = RectFCenter(e->bounds);

    // Spawner behaviour
    for (uint i = 0; i < MAX_SPAWNERS; i++) {
        BulletSpawner& weapon = e->spawner.spawners[i];
        if (weapon.patternToSpawn == NULL || weapon.disabled)
            continue;

        // draw normal (debugging)
        Vec2 weaponCenter = Vec2Add(center, weapon.offset);
        Vec2 end = Vec2Add(weaponCenter, Vec2Scale(weapon.normal, 10.f));
        DrawLine(canvas, V2(weaponCenter), V2(end), 0x0000AAFF);

        // spawn bullets on interval
        if (weapon.interval > 0.f && weapon.spawnTimer > weapon.interval) {
            Entity* bul = CreateEntity(scene);
            InitBullet(bul, weapon.patternToSpawn, weaponCenter, weapon.normal);
            weapon.spawnTimer = 0.f;
        }
        weapon.spawnTimer += delta;
    }

    // Bullet behaviour
    if (RunBulletPattern(e, delta)) {
        DestroyEntity(e);
    }

    // Check collision
    CheckCollisionOfEntity(e, scene);

    // Ship ai behaviour
    RunEntityAI(e, delta);

    // apply movement
    e->bounds.x += vel->x * delta;
    e->bounds.y += vel->y * delta;
}

static usize LastCount = 0;
BULLET usize GetEntityCount()
{
    return LastCount;
}

BULLET void EntityDamage(Entity* e)
{
    auto& health = e->health;
    if (health.value > 0) {
        health.value--;
    } else {
        DestroyEntity(e);
    }
}

BULLET bool EntityIsActive(Entity* e)
{
    return e != NULL && e->isActive;
}

BULLET bool EntityHasFlag(Entity* e, EntityFlag flag)
{
    if (flag == 0) {
        return false;
    }
    return ((e->flags & flag) == flag);
}

BULLET Entity* GetFirstEntityWithFlag(Scene* scene, EntityFlag flag)
{
    for (auto& [id, e] : scene->entities) {
        if (EntityHasFlag(&e, flag) && EntityIsActive(&e)) {
            return &e;
        }
    }
    return NULL;
}

BULLET usize UpdateAndRenderScene(Scene* scene, Texture canvas, float delta)
{
    usize count = 0;
    for (auto& [id, e] : scene->entities) {
        if (e.isActive) {
            UpdateAndRenderEntity(scene, canvas, &e, delta);
            count++;
        }
    }
    LastCount = count;
    return count;
}
