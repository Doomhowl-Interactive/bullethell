#include "basalt.h"
#include "bullet_common.hpp"

typedef struct GUIGUI {
    const LevelInfo* info;
    float timePassed;
} GUIGUI;
static GUIGUI GUI = { 0 };

static void UpdateAndRenderLives(Texture canvas, Entity* player, float delta)
{
    const int size = 64;
    const int border = 10;
    const int spacing = 10;

    Vec2 pos = {
        border,
        HEIGHT - size - border,
    };

    if (!EntityIsActive(player)) {
        return;
    }

    Texture heartTexture = RequestTexture("spr_gui_heart");
    auto& health = player->health;
    for (uint i = 0; i < health.max; i++) {
        DrawTexture(canvas, heartTexture, V2(pos), i < health.value ? WHITE : BLACK);
        pos.x += heartTexture.width + spacing;
    }
}

static void OnLevelEntered(const LevelInfo* info)
{
    SDL_LogInfo(0, "Entered level %s (hooked function)", info->name);
    GUI.info = info;
    GUI.timePassed = 0.0f;
}

static void DrawLevelTitle(Texture canvas, float delta)
{
    const LevelInfo* level = GUI.info;
    if (!level) {
        return;
    }

    if (GUI.timePassed < 2.0f) {
        DrawText(canvas, level->name, 150, 150 + GUI.timePassed * 10.f, 0xFFFF00FF);
    }
}

BULLET void UpdateAndRenderGUI(Texture canvas, Entity* player, float delta)
{
    DrawLevelTitle(canvas, delta);
    UpdateAndRenderLives(canvas, player, delta);

    // hooks
    RunLevelEnterHook(OnLevelEntered);

    GUI.timePassed += delta;
}
