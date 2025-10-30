//
// Created by roki on 2025-10-28.
//

#include "game.h"

#include <cmath>

static constexpr float player_vertical_speed{50.f};
static constexpr float player_boost_speed{150.f};
static constexpr float block_size{40.f};

static constexpr int row_amount{20};
static constexpr int column_amount{14};

static constexpr float board_offset{10.0f};

static board_rectf_t board[row_amount][column_amount] = {};

static float player_position_y{board_offset};
static float player_position_x{board_offset};

static platform::player::block_t player_pattern{block::get_random_block()};
static platform::player::color_t player_color{block::get_random_color()};

static float current_player_speed{0.f};

static bool game_init{true};

void Game::color_bg(const platform::player::color_t& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Game::simulate(const platform::input::input_t& input, const float& delta_time) const
{
    color_bg(platform::player::color_t{181, 175, 174});
    current_player_speed = player_vertical_speed;

    if (IS_DOWN(platform::input::DOWN))
    {
        current_player_speed = player_boost_speed;
    }

    if (IS_PRESSED(platform::input::LEFT))
    {
        player_position_x -= block_size + board_offset;
    }

    if (IS_PRESSED(platform::input::RIGHT))
    {
        player_position_x += block_size + board_offset;
    }

    player_position_y += current_player_speed * delta_time;

    if (game_init)
    {
        for (int row = 0; row < row_amount; ++row)
        {
            for (int col = 0; col < column_amount; ++col)
            {
                const float pos_x = static_cast<float>(col) * block_size +
                    (static_cast<float>(col) * board_offset) + board_offset;
                const float pos_y = static_cast<float>(row) * block_size +
                    (static_cast<float>(row) * board_offset) + board_offset;

                board[row][col] = {{pos_x, pos_y, block_size, block_size}, {191, 232, 88}};
            }
        }

        game_init = false;
    }

    for (int row = 0; row < row_amount; ++row)
    {
        for (int col = 0; col < column_amount; ++col)
        {
            draw_rect(board[row][col].rect, board[row][col].color);
        }
    }

    if (detect_collision())
    {
        SDL_Log("Collision");
        player_pattern = block::get_random_block();
        player_color = block::get_random_color();
        player_position_y = 0;
    }

    draw_player(player_position_x, player_position_y, player_pattern, player_color);

    SDL_RenderPresent(renderer);
}

inline bool Game::detect_collision()
{
    constexpr float cell = block_size + board_offset;

    const int base_col = static_cast<int>(std::round((player_position_x - board_offset) / cell));
    const int base_row = static_cast<int>(std::floor((player_position_y - board_offset) / cell));

    auto is_filled = [](const platform::player::color_t& c) -> bool
    {
        return (c.r == 0 && c.g == 0 && c.b == 0);
    };

    bool will_collide = false;

    for (int r = 0; r < platform::player::PATTERN_ROW; ++r)
    {
        for (int c = 0; c < platform::player::PATTERN_COLUMN; ++c)
        {
            if (player_pattern.pattern[c][r] != '1')
            {
                continue;
            }

            int tile_col = base_col + c;
            const int tile_row = base_row + r;

            if (tile_col < 0) tile_col = 0;
            if (tile_col >= column_amount) tile_col = column_amount - 1;

            const int next_row = tile_row + 1;

            if (next_row >= row_amount)
            {
                will_collide = true;
                break;
            }

            if (is_filled(board[next_row][tile_col].color))
            {
                will_collide = true;
                break;
            }
        }
        if (will_collide) break;
    }

    if (!will_collide)
    {
        return false;
    }

    for (int r = 0; r < platform::player::PATTERN_ROW; ++r)
    {
        for (int c = 0; c < platform::player::PATTERN_COLUMN; ++c)
        {
            if (player_pattern.pattern[c][r] != '1')
            {
                continue;
            }

            int tile_col = base_col + c;
            int tile_row = base_row + r;

            if (tile_col < 0) tile_col = 0;
            if (tile_col >= column_amount) tile_col = column_amount - 1;
            if (tile_row < 0) continue; // Above the board, skip
            if (tile_row >= row_amount) continue; // Safety guard

            board[tile_row][tile_col].color = player_color;
        }
    }

    return true;
}

inline void Game::draw_rect(const SDL_FRect& rect, const platform::player::color_t& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &rect);
}

inline void Game::draw_player(const float& pos_x, const float& pos_y,
                              const platform::player::block_t& block_pattern,
                              const platform::player::color_t& color) const
{
    for (int row = 0; row < platform::player::PATTERN_ROW; ++row)
    {
        for (int col = 0; col < platform::player::PATTERN_COLUMN; ++col)
        {
            if (block_pattern.pattern[col][row] == '1')
            {
                const float tile_x = pos_x + static_cast<float>(col) * block_size + static_cast<float>(col) *
                    board_offset;
                const float tile_y = pos_y + static_cast<float>(row) * block_size + static_cast<float>(row) *
                    board_offset;
                draw_rect({tile_x, tile_y, block_size, block_size}, color);
            }
        }
    }
}
