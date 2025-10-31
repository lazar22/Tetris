//
// Created by roki on 2025-10-28.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#include "platform.h"
#include "block.h"

#define IS_DOWN(button) input.buttons[button].is_down
#define IS_PRESSED(button) (input.buttons[button].is_down && input.buttons[button].changed)
#define IS_RELEASED(button) (!input.buttons[button].is_down && input.buttons[button].changed)

typedef struct BOARD_RECTF
{
    SDL_FRect rect;
    platform::player::color_t color;
} board_rectf_t;

class Game
{
    SDL_Renderer* renderer{nullptr};
    bool is_game_paused{true};

public:
    explicit Game(SDL_Renderer* _renderer) : renderer{_renderer}
    {
    }

    ~Game() = default;

public:
    void simulate(const platform::input::input_t& input, const float& delta_time) const;

    void menu(platform::input::input_t& input, platform::input::mouse_pos_t mouse_pos) const;

    void color_bg(const platform::player::color_t& color) const;

    [[nodiscard]] bool get_game_paused(void) const;

private:
    inline void draw_rect(const SDL_FRect& rect, const platform::player::color_t& color) const;

    inline static bool detect_collision(void);

    static inline bool detect_horizontal_collision(int dx);

    inline void draw_player(const float& pos_x, const float& pos_y,
                            const platform::player::block_t& block_pattern,
                            const platform::player::color_t& color) const;

    inline void draw_text(const uint8_t font_size, const SDL_Rect rect, const SDL_Color txt_color,
                          const char* txt) const;

    static void check_row(void);

    inline void change_game_state(void);
};

#endif //GAME_H
