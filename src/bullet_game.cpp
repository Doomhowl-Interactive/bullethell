#include "basalt.h"
#include "basalt_extra.h"
#include "basalt_extra.hpp"

#include "bullet_common.hpp"

#include <deque>
#include <math.h>
#include <vector>
#include <stdio.h>

#define SKY_COLOR 0x000000FF
#define SCENE_GAME 0
#define SCENE_BULLET_PREVIEW 1
#define SCENE_COUNT 2

using namespace std;

static uint SelectedPattern = 1;

#define TEST_ENEMY_COUNT 10
static vector<EntityID> TestEnemies;

static deque<Scene> Scenes;

Texture BulletPlacholderTexture = { 0 };
Texture PlayerTexture = { 0 };

usize GameDifficulty = 2;

DYNAMIC BASALT GameConfig ConfigureGame()
{
    GameConfig config;
    config.title = GetRealRandomNumber() % 1000 == 699 ? "Guardians Of Ur Mum"
                                                       : "Guardians of the Holy Fire";
    config.company = "Doomhowl Interactive";
    config.width = WIDTH;
    config.height = HEIGHT;
    config.language = 2;
    return config;
}

DYNAMIC BASALT void InitializeGame()
{
    // if (Config.hasUnitTesting) // TODO DOESN'T RUN
    UnitTestBullet();

    BulletPlacholderTexture = RequestTexture("SPR_BULLET_PLACEHOLDER");
    PlayerTexture = RequestTexture("SPR_SHIP_PLAYER");

    // Create game scene
    Scenes.push_front({ "Game", {} });

    Entity* player = CreateEntity(&Scenes.front());
    Vec2 spawnPos = { WIDTH / 2.0f, HEIGHT / 1.2f };
    InitPlayer(player, spawnPos);

    SwitchLevel(&Level1);
    ConstructDialogs();

    // StartDialogSequence("TUTORIAL_START");
}

DYNAMIC BASALT void DisposeGame()
{
    SDL_LogWarn(0, "TODO: dispose game properly");
}

DYNAMIC BASALT void UpdateAndRenderGame(Texture canvas, float delta)
{
    if (IsKeyPressed(SDLK_i)) {
        auto front = Scenes.front();
        Scenes.pop_front();
        Scenes.push_back(front);
    }
    if (IsKeyPressed(SDLK_u)) {
        auto back = Scenes.back();
        Scenes.pop_back();
        Scenes.push_front(back);
    }

    if (IsKeyPressed(SDLK_m)) {
        TakeScreenshot(canvas);
    }

    if (DialogIsSpeaking()) {
        delta = 0.f;
    }

    Scene* activeScene = GetActiveScene();
    UpdateAndRenderLevel(canvas, activeScene, delta);

    Entity* player = GetFirstEntityWithFlag(activeScene, FLAG_PLAYER);
    UpdateAndRenderGUI(canvas, player, delta);
    UpdateAndRenderDialogBoxes(canvas);

    // draw fps
    float fps = 1.f / delta;
    const char* infoText = FormatText("FPS %1.3f", fps);
    DrawText(canvas, infoText, 10, HEIGHT - 20, fps < 30 ? RED : GREEN);
}

Scene* GetActiveScene()
{
    return &Scenes.front();
}
