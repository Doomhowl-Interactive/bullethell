#include "bullet_scenes.hpp"

#include "bullet_entities.hpp"

void Scene::ClearEntities()
{
    entities.clear();
    TraceLog(LOG_INFO, "Cleared all entities!");
}

Entity* Scene::CreateEntity()
{
    size_t nextID = entities.size();
    Entity entity(nextID, this);
    entities.insert({ nextID, entity });
    return &entities[nextID];
}

Entity* Scene::GetFirstEntityWithFlag(EntityFlag flag)
{
    for (auto& [id, entity] : entities) {
        if (entity.isActive && entity.HasFlag(flag)) {
            return &entity;
        }
    }
    return nullptr;
}

void Scene::Update(float delta)
{
    for (auto& [id, entity] : entities) {
        if (!entity.isActive) {
            continue;
        }
        entity.Update(delta);
        for (auto& [otherID, otherEntity] : entities) {
            entity.CheckCollision(&otherEntity);
        }
    }
}

void Scene::Draw()
{
    for (auto& [id, entity] : entities) {
        if (entity.isActive) {
            entity.Draw();
        }
    }
}

size_t Scene::GetEntityCount() const
{
    return entities.size();
}
