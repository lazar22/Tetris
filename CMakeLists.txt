cmake_minimum_required(VERSION 3.14)

project(Tetris)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(Tetris src/main.cpp
        lib/game/game.cpp
        include/platform.h)

target_include_directories(Tetris PRIVATE include lib/game)

target_link_libraries(Tetris ${SDL2_LIBRARIES})