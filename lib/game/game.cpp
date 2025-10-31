//
// Created by roki on 2025-10-28.
//

#include "game.h"

#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <cmath>

static constexpr float player_vertical_speed{50.f};
static constexpr float player_boost_speed{150.f};
static constexpr float block_size{40.f};

static constexpr int row_amount{20};
static constexpr int column_amount{14};

static constexpr float board_offset{10.0f};

static auto font_path = "assets/fonts/Debrosee_ALPnL.ttf";

// Color used to represent an empty board cell
static constexpr platform::player::color_t empty_color{191, 232, 88};

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

    if (IS_PRESSED(platform::input::LEFT) && detect_horizontal_collision(-1))
    {
        player_position_x -= block_size + board_offset;
    }

    if (IS_PRESSED(platform::input::RIGHT) && detect_horizontal_collision(1))
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

                board[row][col] = {{pos_x, pos_y, block_size, block_size}, empty_color};
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
        check_row();

        player_pattern = block::get_random_block();
        player_color = block::get_random_color();

        player_position_y = board_offset;
    }

    draw_player(player_position_x, player_position_y, player_pattern, player_color);
}

void Game::menu(platform::input::input_t& input, const platform::input::mouse_pos_t mouse_pos) const
{
    color_bg(platform::player::color_t{181, 175, 174});

    constexpr SDL_Color def_txt_color{255, 255, 255};
    constexpr SDL_Color sel_txt_color{128, 224, 157};

    SDL_Color cur_start_color;
    SDL_Color cur_quit_color;

    constexpr int def_txt_size = 30;

    constexpr int txt_w = 200;
    constexpr int txt_h = 50;

    constexpr int offset = 20;

    constexpr int center_x = (platform::window::width - txt_w) / 2;
    constexpr int center_y = (platform::window::height) / 2 - txt_h * 2;

    constexpr SDL_Rect start_rect{center_x, center_y, txt_w, txt_h};
    constexpr SDL_Rect quit_rect{center_x, center_y + txt_h + offset, txt_w, txt_h};

    bool is_mouse_over_start = false;
    bool over_quit = false;
    bool over_start = false;

    if (mouse_pos.x >= start_rect.x &&
        mouse_pos.x <= start_rect.x + start_rect.w &&
        mouse_pos.y >= start_rect.y &&
        mouse_pos.y <= start_rect.y + start_rect.h)
    {
        cur_start_color = sel_txt_color;
        over_start = true;

        if (IS_PRESSED(platform::input::MOUSE_BTN_LEFT))
        {
            // Start Game
            // is_game_paused = false;
        }
    }
    else
    {
        cur_start_color = def_txt_color;
        over_start = false;
    }

    if (mouse_pos.x >= quit_rect.x &&
        mouse_pos.x <= quit_rect.x + quit_rect.w &&
        mouse_pos.y >= quit_rect.y &&
        mouse_pos.y <= quit_rect.y + quit_rect.h)
    {
        cur_quit_color = sel_txt_color;
        over_quit = true;

        if (IS_PRESSED(platform::input::MOUSE_BTN_LEFT))
        {
            // Quit Game
        }
    }
    else
    {
        cur_quit_color = def_txt_color;
        over_quit = false;
    }

    is_mouse_over_start = over_start || over_quit;

    SDL_SetCursor(is_mouse_over_start
                      ? SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)
                      : SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

    draw_text(def_txt_size,
              start_rect,
              cur_start_color,
              "Start");

    draw_text(def_txt_size,
              quit_rect,
              cur_quit_color,
              "Quit");
}

bool Game::get_game_paused(void) const
{
    return is_game_paused;
}

inline bool Game::detect_collision()
{
    constexpr float cell = block_size + board_offset;

    const int base_col = static_cast<int>(std::round((player_position_x - board_offset) / cell));
    const int base_row = static_cast<int>(std::floor((player_position_y - board_offset) / cell));

    auto is_filled = [](const platform::player::color_t& c) -> bool
    {
        return !(c.r == empty_color.r && c.g == empty_color.g && c.b == empty_color.b);
    };

    bool will_collide = false;
    bool collided_with_floor = false;

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
                collided_with_floor = true;
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

    const int placement_base_row = base_row + (collided_with_floor ? -1 : 0);

    for (int r = 0; r < platform::player::PATTERN_ROW; ++r)
    {
        for (int c = 0; c < platform::player::PATTERN_COLUMN; ++c)
        {
            if (player_pattern.pattern[c][r] != '1')
            {
                continue;
            }

            int tile_col = base_col + c;
            const int tile_row = placement_base_row + r;

            if (tile_col < 0) tile_col = 0;
            if (tile_col >= column_amount) tile_col = column_amount - 1;
            if (tile_row < 0) continue;
            if (tile_row >= row_amount) continue;

            board[tile_row][tile_col].color = player_color;
        }
    }

    return true;
}

inline bool Game::detect_horizontal_collision(int dx)
{
    constexpr float cell = block_size + board_offset;

    const int base_col = static_cast<int>(std::round((player_position_x - board_offset) / cell));
    const int base_row = static_cast<int>(std::floor((player_position_y - board_offset) / cell));

    auto is_filled = [](const platform::player::color_t& c) -> bool
    {
        return !(c.r == empty_color.r && c.g == empty_color.g && c.b == empty_color.b);
    };

    for (int r = 0; r < platform::player::PATTERN_ROW; ++r)
    {
        for (int c = 0; c < platform::player::PATTERN_COLUMN; ++c)
        {
            if (player_pattern.pattern[c][r] != '1')
            {
                continue;
            }

            const int tile_row = base_row + r;
            const int target_col = base_col + c + dx;

            if (tile_row < 0 || tile_row >= row_amount)
            {
                continue;
            }

            if (target_col < 0 || target_col >= column_amount)
            {
                return false;
            }

            if (is_filled(board[tile_row][target_col].color))
            {
                return false;
            }
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

inline void Game::draw_text(const uint8_t font_size, const SDL_Rect rect, const SDL_Color txt_color,
                            const char* txt) const
{
    TTF_Font* font = TTF_OpenFont(font_path, font_size);
    SDL_Surface* surface_msg = TTF_RenderText_Solid(font, txt, txt_color);

    SDL_Texture* texture_msg = SDL_CreateTextureFromSurface(renderer, surface_msg);
    SDL_RenderCopy(renderer, texture_msg, nullptr, &rect);

    TTF_CloseFont(font);
}

void Game::check_row(void)
{
    auto is_filled = [](const platform::player::color_t& c) -> bool
    {
        return !(c.r == empty_color.r && c.g == empty_color.g && c.b == empty_color.b);
    };

    for (int row = 0; row < row_amount; ++row)
    {
        bool full = true;
        for (int col = 0; col < column_amount; ++col)
        {
            if (!is_filled(board[row][col].color))
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int col = 0; col < column_amount; ++col)
            {
                board[row][col].color = empty_color;
            }
        }
    }
}

void Game::change_game_state()
{
    is_game_paused = !is_game_paused;
}

