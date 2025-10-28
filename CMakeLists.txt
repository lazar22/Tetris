cmake_minimum_required(VERSION 3.20)
project(Tetris)

IF (WIN32)
    find_package(SDL2 CONFIG REQUIRED)
ELSE ()
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
ENDIF ()

add_executable(Tetris src/main.cpp lib/game/game.cpp include/platform.h)
target_include_directories(Tetris PRIVATE include lib/game)

IF (WIN32)
    target_link_libraries(Tetris PRIVATE SDL2::SDL2main SDL2::SDL2)
ELSE ()
    target_link_libraries(Tetris ${SDL2_LIBRARIES})
ENDIF ()