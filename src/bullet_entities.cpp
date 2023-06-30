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
    SDL_LogInfo(0, "Cleared all entities!");
}

// TODO: constructor
static void InitEntity(Entity* entity, Scene* scene)
{
    static usize nextID = 0;
    entity->id = nextID++;
    entity->scene = scene;
    entity->isActive = true;
    entity->frameInterval = 0.2f;
    entity->tint = WHITE;
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

    // Entity drawing
    if (e->timer > e->frameInterval) {
        e->timer = 0.f;
        e->frameID++;
    }
    e->timer += delta;

    if (e->texture.width > 0) {
        if (e->texture.pixels) {
            DrawTextureEx(canvas,
                          e->texture,
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

    // WEAPON BEHAVIOUR
    for (uint i = 0; i < MAX_SPAWNERS; i++) {
        BulletSpawner* weapon = &e->bulletSpawners[i];
        if (weapon->patternToSpawn == NULL || weapon->disabled)
            continue;

        // draw normal (debugging)
        Vec2 weaponCenter = Vec2Add(center, weapon->offset);
        Vec2 end = Vec2Add(weaponCenter, Vec2Scale(weapon->normal, 10.f));
        DrawLine(canvas, V2(weaponCenter), V2(end), 0x0000AAFF);

        // spawn bullets on interval
        if (weapon->interval > 0.f && weapon->spawnTimer > weapon->interval) {
            Entity* bul = CreateEntity(scene);
            InitBullet(bul, weapon->patternToSpawn, weaponCenter, weapon->normal);
            weapon->spawnTimer = 0.f;
        }
        weapon->spawnTimer += delta;
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
    if (e->health > 0) {
        e->health--;
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
