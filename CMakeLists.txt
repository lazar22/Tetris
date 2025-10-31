cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Tetris)

IF (WIN32)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
ELSE ()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)

    include_directories(${SDL2_INCLUDE_DIR})
ENDIF ()

add_executable(Tetris src/main.cpp
        include/platform.h
        lib/game/game.cpp
        lib/block/block.cpp)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Tetris>/assets"
        COMMENT "Copying assets to $<TARGET_FILE_DIR:Tetris>/assets"
)

add_dependencies(Tetris copy_assets)

target_include_directories(Tetris PRIVATE include lib/game lib/block)

IF (WIN32)
    target_link_libraries(Tetris PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_ttf::SDL2_ttf)
ELSE ()
    target_link_libraries(Tetris ${SDL2_LIBRARIES})
ENDIF ()

