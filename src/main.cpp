//
// Created by roki on 2025-10-28.
//
#include <string>
#include <SDL2/SDL.h>

#include "platform.h"
#include "game.h"

static platform::input::input_t input;

static float delta_time = 0.f;

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(platform::window::title.c_str(), 0, 0,
                                          platform::window::width, platform::window::height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool is_running = true;
    SDL_Event event;

    const Game game{renderer};

    unsigned long long last_time = SDL_GetPerformanceCounter();
    while (is_running) {
        for (int i = 0; i < platform::input::BTN_COUNT; ++i) {
            input.buttons[i].changed = false;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }

            switch (event.type) {
                case SDL_KEYUP:
                case SDL_KEYDOWN: {
                    const unsigned int key = event.key.keysym.sym;
                    const bool is_down = (event.type == SDL_KEYDOWN);

                    switch (key) {
                        READ_KEY(platform::input::DOWN, SDLK_DOWN);
                        READ_KEY(platform::input::LEFT, SDLK_LEFT);
                        READ_KEY(platform::input::RIGHT, SDLK_RIGHT);

                        default: break ;
                    }
                }
                break;

                default:
                    break;
            }
        }

        const unsigned long long now_time = SDL_GetPerformanceCounter();
        delta_time = static_cast<float>(now_time - last_time) / static_cast<float>(SDL_GetPerformanceFrequency());
        last_time = now_time;

        game.simulate(input, delta_time);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
