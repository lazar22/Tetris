//
// Created by roki on 2025-10-28.
//

#include "game.h"

#define IS_DOWN(button) input.buttons[button].is_down
#define IS_PRESSED(button) (input.buttons[button].is_down && input.buttons[button].changed)
#define IS_RELEASED(button) (!input.buttons[button].is_down && input.buttons[button].changed)

static constexpr float player_horizontal_speed{5.f};
static constexpr float player_vertical_speed{50.f};
static constexpr float block_size{50.f};

static float player_position_y{150};
static float player_position_x{150};

void Game::color_bg(const color_t &color) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Game::simulate(const platform::input::input_t &input, const float &delta_time) const {
    color_bg(color_t{181, 175, 174});

    if (IS_DOWN(platform::input::DOWN)) {
        player_position_y += player_vertical_speed * delta_time;
    }

    if (IS_PRESSED(platform::input::LEFT)) {
        player_position_x -= block_size;
    }

    if (IS_PRESSED(platform::input::RIGHT)) {
        player_position_x += block_size;
    }

    draw_rect({player_position_x, player_position_y, block_size, block_size}, {232, 110, 88});

    SDL_RenderPresent(renderer);
}

inline void Game::draw_rect(const SDL_FRect &rect, const Color &color) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &rect);
}
