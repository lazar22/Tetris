//
// Created by roki on 2025-10-28.
//
#ifndef PLATFORM_H
#define PLATFORM_H

#define READ_KEY(button, key) case(key):{\
    input.buttons[button].changed = is_down != input.buttons[button].is_down;\
    input.buttons[button].is_down = is_down;\
} break;

#include <string>
#include <cstdint>

namespace platform
{
    namespace player
    {
        constexpr uint8_t PATTERN_COLUMN{2};
        constexpr uint8_t PATTERN_ROW{4};

        typedef struct COLOR
        {
            unsigned r{};
            unsigned g{};
            unsigned b{};
            unsigned a{255};
        } color_t;

        typedef struct BLOCK
        {
            char pattern[PATTERN_COLUMN][PATTERN_ROW];
        } block_t;
    }

    namespace window
    {
        const std::string title = "Tetris";
        constexpr unsigned width{710};
        constexpr unsigned height{960};
    }

    namespace input
    {
        typedef struct BUTTON_STATE
        {
            bool is_down{false};
            bool changed{false};
        } btn_state_t;


        enum BUTTONS
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,

            QUIT,

            BTN_COUNT,
        };

        typedef struct INPUT
        {
            BUTTON_STATE buttons[BTN_COUNT];
        } input_t;
    }
}

#endif
