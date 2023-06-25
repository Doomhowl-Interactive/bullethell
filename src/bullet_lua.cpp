#include <iostream>
#include <SDL_log.h>
#include "bullet_common.h"

extern "C" {
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>
}

using namespace std;

BULLET void RunLuaExample()
{
    string cmd = "a = 7 + 11";

    lua_State* L = luaL_newstate();

    int r = luaL_dostring(L, cmd.c_str());

    if (r == 0) {
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1)) {
            int result = lua_tonumber(L, -1);
            SDL_LogInfo(0, "LUA OK: %d", result);
        } else {
            SDL_LogError(0, "LUA: No number on stack");
        }
    } else {
        string errorMsg = lua_tostring(L, -1);
        SDL_LogError(0, "LUA: %s", errorMsg.c_str());
    }

    lua_close(L);
}