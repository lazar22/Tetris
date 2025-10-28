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

namespace platform {
    namespace window {
        const std::string title = "Tetris";
        constexpr unsigned width{640};
        constexpr unsigned height{480};
    }

    namespace input {
        typedef struct BUTTON_STATE {
            bool is_down;
            bool changed;
        } btn_state_t;


        enum BUTTONS {
            UP,
            DOWN,
            LEFT,
            RIGHT,

            BTN_COUNT,
        };

        typedef struct INPUT {
            BUTTON_STATE buttons[BTN_COUNT];
        } input_t;
    }
}

#endif
