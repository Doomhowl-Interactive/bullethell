cmake_minimum_required(VERSION 3.10)
project(bullet_game)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_GAME *.c* *.h*)

add_compile_options(-Wall -Wextra -Wpedantic -fpermissive)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_GAME})
    set(SDL2_DIR_ARCH ${CMAKE_SOURCE_DIR}/vendor/basalt/vendor/SDL2-2.26.5/x86_64-w64-mingw32)
    set(SDL2_DIR ${SDL2_DIR_ARCH}/../cmake)
else()
    add_executable(${PROJECT_NAME} ${SRC_GAME})
endif()

target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES} basalt)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/basalt/src)
