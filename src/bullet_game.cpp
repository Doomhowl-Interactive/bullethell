#include <vector>

#include "bullet_assets.hpp"
#include "bullet_common.hpp"
#include "bullet_dialog.hpp"

using namespace std;

int main()
{
    const int screenWidth = WIDTH;
    const int screenHeight = HEIGHT;

    uint ActiveSceneID = 0;

    vector<Scene> scenes;
    scenes.emplace_back();

    Entity* player = CreateEntity(&Scenes[SCENE_GAME]);
    Vector2 spawnPos = { WIDTH / 2.0f, HEIGHT / 1.2f };
    InitPlayer(player, spawnPos);

    auto assets = Assets::Initialize();
    auto dialog = Dialog::Initialize();

    SwitchLevel(&Level1);
    Dialog::StartSequence("TUTORIAL_START");

    InitWindow(screenWidth, screenHeight, "Guardians of the Holy Fire");

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_I)) {
            ActiveSceneID++;
            if (ActiveSceneID >= SCENE_COUNT)
                ActiveSceneID = 0;
        }
        if (IsKeyPressed(KEY_U)) {
            ActiveSceneID--;
            if (ActiveSceneID < 0)
                ActiveSceneID = SCENE_COUNT - 1;
        }

        Scene* activeScene = &Scenes[ActiveSceneID];
        UpdateAndRenderLevel(activeScene, delta);

        Entity* player = GetFirstEntityWithFlag(activeScene, FLAG_PLAYER);
        UpdateAndRenderGUI(player, delta);
        dialog->UpdateAndRenderBoxes();

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}