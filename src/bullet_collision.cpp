#include "basalt.h"
#include "bullet_common.hpp"

static void OnTouchedEntity(Entity* e, Entity* sender)
{
    if (EntityHasFlag(sender, FLAG_BULLET)) {
        if (EntityHasFlag(e, FLAG_PLAYER)) {
            EntityDamage(e);
        }
        if (EntityHasFlag(e, FLAG_ENEMY)) {
            EntityDamage(e);
        }
    }
}

BULLET void CheckCollisionOfEntity(Entity* sender, Scene* scene)
{
    auto& col = sender->collision;
    if (!col.toucher) {
        return;
    }

    for (auto& [id, listener] : scene->entities) {
        if (!listener.isActive || sender->id == listener.id) {
            continue;
        }

        if (EntityHasFlag(&listener, col.ignoreFlags)) {
            return;
        }
        if (RectFOverlaps(sender->bounds, listener.bounds)) {
            OnTouchedEntity(&listener, sender);
        }
    }
}
