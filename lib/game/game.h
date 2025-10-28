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

typedef struct Color
{
    unsigned r{};
    unsigned g{};
    unsigned b{};
    unsigned a{255};
} color_t;

typedef struct BOARD_RECTF
{
    SDL_FRect rect;
    color_t color;
} board_rectf_t;

class Game
{
    SDL_Renderer* renderer{nullptr};

public:
    explicit Game(SDL_Renderer* _renderer) : renderer{_renderer}
    {
    }

    ~Game() = default;

public:
    void simulate(const platform::input::input_t& input, const float& delta_time) const;

    void color_bg(const color_t& color) const;

private:
    inline void draw_rect(const SDL_FRect& rect, const Color& color) const;

    inline static bool detect_collision(void);

    inline void draw_player(const float& pos_x, const float& pos_y,
                            const block_t& block_pattern, const color_t& player_color) const;
};

#endif //GAME_H
