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

static block_t player_pattern{block::get_random_block()};

static float current_player_speed{0.f};

static bool game_init{true};

void Game::color_bg(const color_t& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Game::simulate(const platform::input::input_t& input, const float& delta_time) const
{
    color_bg(color_t{181, 175, 174});
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
        player_position_y = 0;
    }

    draw_player(player_position_x, player_position_y, player_pattern, {232, 110, 88});

    SDL_RenderPresent(renderer);
}

inline bool Game::detect_collision()
{
    constexpr float cell = block_size + board_offset;

    int col = static_cast<int>(std::round((player_position_x - board_offset) / cell));
    const int row = static_cast<int>(std::floor((player_position_y - board_offset) / cell));

    if (col < 0)
    {
        col = 0;
    }

    if (col >= column_amount)
    {
        col = column_amount - 1;
    }

    const int next_row = row + 1;

    if (next_row >= row_amount - 1)
    {
        board[row_amount - 2][col].color = {0, 0, 0};
        return true;
    }

    const color_t& below = board[next_row][col].color;
    const bool below_filled = (below.r == 0 && below.g == 0 && below.b == 0);

    if (below_filled)
    {
        if (row > 0 && row < row_amount - 2)
        {
            board[row][col].color = {0, 0, 0};
            return true;
        }
    }
    return false;
}

inline void Game::draw_rect(const SDL_FRect& rect, const Color& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &rect);
}

inline void Game::draw_player(const float& pos_x, const float& pos_y,
                              const block_t& block_pattern, const color_t& player_color) const
{
    for (int row = 0; row < PATTERN_ROW; ++row)
    {
        for (int col = 0; col < PATTERN_COLUMN; ++col)
        {
            if (block_pattern.pattern[col][row] == '1')
            {
                const float tile_x = pos_x + static_cast<float>(col) * block_size + static_cast<float>(col) *
                    board_offset;
                const float tile_y = pos_y + static_cast<float>(row) * block_size + static_cast<float>(row) *
                    board_offset;
                draw_rect({tile_x, tile_y, block_size, block_size}, player_color);
            }
        }
    }
}
