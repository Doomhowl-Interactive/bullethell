#include "bullet_lua_console.hpp"
#include "basalt.h"

BULLET void UpdateAndRenderLuaConsole(Texture canvas, float delta)
{
    const char* error = GetLuaErrors();
    DrawText(canvas, error, 10, 250, 0x0000FFFF);
}
