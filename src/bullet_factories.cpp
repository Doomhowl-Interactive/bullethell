#include <math.h>

#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.hpp"

void InitPlayerLevel1Weapon(Entity* e)
{
    float distanceFromPlayer = 45;
    float spaceBetween = 45;

    Vec2 offset = { -spaceBetween * 0.5f, -distanceFromPlayer };

    for (int i = 0; i < 2; i++) {
        BulletSpawner& spawner = e->spawner.spawners[i];
        spawner.interval = 0.1f;
        spawner.normal.x = 0;
        spawner.normal.y = -1;
        spawner.offset = offset;
        spawner.patternToSpawn = GetBulletPatternByName("PlayerBullet1");
        offset.x += spaceBetween;
    }
}

void InitPlayerLevel4Weapon(Entity* e)
{
    // Bullet spawners
    double outwardsAngleDeg = 40;
    double distanceFromPlayer = 45;
    uint spawnerCount = 5;

    double anglePerSpawner = outwardsAngleDeg / spawnerCount;
    for (uint i = 0; i < spawnerCount; i++) {
        double angle = -90 - outwardsAngleDeg * 0.5f + anglePerSpawner * i + anglePerSpawner * 0.5f;

        BulletSpawner& spawner = e->spawner.spawners[i];
        spawner.interval = 0.1f;
        spawner.normal.x = cos(DEG2RAD(angle));
        spawner.normal.y = sin(DEG2RAD(angle));
        spawner.offset = Vec2Scale(spawner.normal, distanceFromPlayer);
        spawner.patternToSpawn = GetBulletPatternByName("PlayerBullet1");
    }
}

BULLET void InitPlayer(Entity* e, Vec2 pos)
{
    SDL_LogDebug(0, "Spawned player at %f %f", pos.x, pos.y);
    e->tint = WHITE;
    e->flags = FLAG_PLAYER;
    e->sprite.texture = RequestTexture("SPR_SHIP_PLAYER");
    e->ship.moveSpeed = 200;

    SetEntityCenter(e, pos.x - 48.f / 2.f, pos.y);
    SetEntitySize(e, e->sprite.texture.width, e->sprite.texture.height);
    assert(e->bounds.width > 0 && e->bounds.height > 0);

    // Health
    e->health.max = 3;
    e->health.value = 3;

    // "AI" Behaviour
    e->ai.behaviour = *GetEntityAIByName("PlayerMovement");

    InitPlayerLevel1Weapon(e);
}

BULLET void InitTestEnemy(Entity* e, Vec2 pos)
{
    SDL_LogDebug(0, "Spawned enemy at %f %f", pos.x, pos.y);
    e->sprite.texture = RequestTexture("SPR_SHIP_PLAYER");
    SetEntityCenter(e, pos.x - 48.f / 2.f, pos.y);
    e->tint = RED;
    e->flags = FLAG_ENEMY;

    e->health.set(10);

    SetEntitySize(e, e->sprite.texture.width, e->sprite.texture.height);
    assert(e->bounds.width > 0 && e->bounds.height > 0);

    // Move downwards
    e->vel.y = 80;

    // Bullet spawner
    BulletSpawner& spawner = e->spawner.spawners[0];
    spawner.interval = 0.5f;
    spawner.normal = Vec2Normalize(e->vel);
    spawner.offset = Vec2Scale(spawner.normal, 50);
    spawner.patternToSpawn = GetBulletPatternByName("SimpleForwards");
}

BULLET void InitBullet(Entity* e, const BulletPattern* pattern, Vec2 pos, Vec2 normal)
{
    assert(e);
    e->collision.toucher = true;
    e->flags = FLAG_BULLET;

    // set sprite
    const char* texture = pattern->texture == NULL ? "SPR_BULLET_PLACEHOLDER" : pattern->texture;
    auto& sprite = e->sprite;
    sprite.texture = RequestTexture(texture);
    SetEntityCenter(e, pos.x, pos.y);
    SetEntitySize(e, sprite.texture.width, sprite.texture.height);
    sprite.sourceOffset.x = 0;
    sprite.sourceOffset.y = 0;

    // copy bullet pattern
    auto& bullet = e->bullet;
    bullet.pattern = *pattern;
    bullet.pattern.data.origin = pos;
    bullet.pattern.data.normal = normal;
    e->tint = 0xFFFFFFFF;
}
