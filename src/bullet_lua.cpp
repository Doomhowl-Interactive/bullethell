#include <iostream>
#include "bullet_common.h"

extern "C" {
    #include <lua5.1/lua.h>
    #include <lua5.1/lauxlib.h>
    #include <lua5.1/lualib.h>
}

using namespace std;

BULLET void RunLuaExample(){
    std::cout << "Hello world!";
}