cmake_minimum_required(VERSION 3.10)
project(bullet_game)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_GAME *.c *.cpp *.h *.hpp)

link_libraries(basalt bulletml)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_GAME})
    set(SDL2_DIR_ARCH ${CMAKE_SOURCE_DIR}/vendor/basalt/vendor/SDL2-2.26.5/x86_64-w64-mingw32)
	set(SDL2_DIR ${SDL2_DIR_ARCH}/../cmake)
    file(COPY ${SDL2_DIR_ARCH}/bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
    add_executable(${PROJECT_NAME} ${SRC_GAME})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/basalt/src bulletml)