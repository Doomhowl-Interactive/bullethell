#include "bullet_common.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

BULLET void RunLuaFile(const char* filename);
BULLET const char* GetLuaErrors();

#ifdef __cplusplus
}
#endif