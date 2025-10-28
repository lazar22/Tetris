cmake_minimum_required(VERSION 3.14)

project(Tetris)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(tetris src/main.cpp)
target_link_libraries(tetris ${SDL2_LIBRARIES})