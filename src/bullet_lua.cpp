#include <algorithm>
#include <string.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <SDL_log.h>

#include "bullet_lua.h"
#include "lua.h"

namespace fs = std::filesystem;
using namespace std;

static const vector<fs::path> LuaFolders = { "lua", "../lua", "../../lua", "../../../lua" };

static lua_State* EnsureVM()
{
    static lua_State* L = nullptr;
    if (L == nullptr) {
        L = luaL_newstate();
        luaL_openlibs(L);
    }
    return L;
}

BULLET const char* GetLuaErrors()
{
    static const int BUFFER_SIZE = 1024;
    static char buffer[BUFFER_SIZE] = { 0 };

    auto vm = EnsureVM();
    if (vm != LUA_OK) {
        const char* s = lua_tostring(vm, -1);
        strncpy(buffer, s, BUFFER_SIZE);
    }
    return buffer;
}

static bool SearchLuaFile(const char* fileName, fs::path* outPath)
{
    auto filePathWithExt = fs::path(fileName).replace_extension(".lua");
    for (const auto& folder : LuaFolders) {
        auto filePath = folder / filePathWithExt;
        if (fs::exists(filePath)) {
            *outPath = filePath;
            return true;
        }
    }
    return false;
}

BULLET void RunLuaFile(const char* fileName)
{
    auto vm = EnsureVM();

    // Find lua file
    fs::path filePath;
    if (SearchLuaFile(fileName, &filePath)) {
    } else {
        SDL_LogError(0, "Could not find lua file %s", fileName);
    }

    // Run lua file
    if (luaL_dofile(vm, filePath.string().c_str()) == LUA_OK) {
        SDL_LogDebug(0, "Ran lua file %s", fileName);
    } else {
        SDL_LogError(0, "Could not run lua file %s: %s", fileName, GetLuaErrors());
    }
}