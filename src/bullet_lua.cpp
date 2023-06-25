#include <algorithm>
#include <string.h>
#include <iostream>

#include "bullet_lua.h"
#include "lua.h"

using namespace std;

static lua_State* L;

BULLET void InitLuaVM()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

BULLET const char* GetLuaErrors()
{
    const int BUFFER_SIZE = 1024;
    static char buffer[BUFFER_SIZE] = { 0 };
    if (L != LUA_OK) {
        const char* s = lua_tostring(L, -1);
        strncpy(buffer, s, BUFFER_SIZE);
    }
    return buffer;
}