#pragma once
#include <memory>
#include <unordered_map>

#include "bullet_common.hpp"
#include "bullet_entities.hpp"

class Scene : public UpdateAndDrawable {
   public:
    void ClearEntities();
    Entity* CreateEntity();

    Entity* GetFirstEntityWithFlag(EntityFlag flag);

    void Update(float delta) override;
    void Draw() override;

    size_t GetEntityCount() const;

   private:
    std::unordered_map<size_t, Entity> entities;
};
