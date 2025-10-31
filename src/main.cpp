//
// Created by roki on 2025-10-28.
//
#include <SDL_ttf.h>
#include <string>
#include <SDL2/SDL.h>

#include "platform.h"
#include "game.h"

static platform::input::input_t input;

static float delta_time = 0.f;

static bool is_paused{true};

#if WIN32
int SDL_main(int argc, char** argv)
#else
int main()
#endif
{
    SDL_Init(SDL_INIT_EVERYTHING);
    if (TTF_Init() != 0)
    {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return 1;
    }

    SDL_Rect window_rect;
    SDL_GetDisplayBounds(0, &window_rect);

    const unsigned pos_win_x = window_rect.x + (window_rect.w - platform::window::width) / 2;
    const unsigned pos_win_y = window_rect.y + (window_rect.h - platform::window::height) / 2;

    SDL_Window* window = SDL_CreateWindow(platform::window::title.c_str(),
                                          static_cast<int>(pos_win_x), static_cast<int>(pos_win_y),
                                          platform::window::width, platform::window::height, SDL_WINDOW_SHOWN);
    SDL_SetWindowBordered(window, SDL_TRUE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool is_running = true;
    SDL_Event event;

    const Game game{renderer};

    unsigned long long last_time = SDL_GetPerformanceCounter();
    while (is_running)
    {
        for (int i = 0; i < platform::input::BTN_COUNT; ++i)
        {
            input.buttons[i].changed = false;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }

            switch (event.type)
            {
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                {
                    const unsigned int key = event.key.keysym.sym;
                    const bool is_down = (event.type == SDL_KEYDOWN);

                    switch (key)
                    {
                    READ_KEY(platform::input::DOWN, SDLK_DOWN);
                    READ_KEY(platform::input::LEFT, SDLK_LEFT);
                    READ_KEY(platform::input::RIGHT, SDLK_RIGHT);

                    READ_KEY(platform::input::QUIT, SDLK_ESCAPE);

                    default: break ;
                    }
                }
                break;

            default:
                break;
            }
        }

        if (IS_PRESSED(platform::input::QUIT))
        {
            is_running = false;
            break;
        }

        const unsigned long long now_time = SDL_GetPerformanceCounter();
        delta_time = static_cast<float>(now_time - last_time) / static_cast<float>(SDL_GetPerformanceFrequency());
        last_time = now_time;

        if (is_paused)
        {
            game.menu(input, window_rect);
        }
        else
        {
            game.simulate(input, delta_time);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
    TTF_Quit();

    return 0;
}
