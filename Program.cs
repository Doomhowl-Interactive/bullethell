using Raylib_cs;

Raylib.InitWindow(800, 600, "Guardians of the Holy Fire");

while (!Raylib.WindowShouldClose())
{
    Raylib.BeginDrawing();
    Raylib.ClearBackground(Color.WHITE);

    Raylib.DrawText("Hello, world!", 12, 12, 20, Color.BLACK);

    Raylib.EndDrawing();
}

Raylib.CloseWindow();