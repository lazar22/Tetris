//
// Created by roki on 2025-10-28.
//
#include <string>
#include <SDL2/SDL.h>

#include "platform.h"
#include "game.h"

static platform::input::input_t input;

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(platform::window::title.c_str(), 0, 0,
                                          platform::window::width, platform::window::height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool is_running = true;
    SDL_Event event;

    while (is_running) {
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
                        READ_KEY(platform::input::UP, SDLK_UP);
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

        Game game{renderer};
        game.color_bg(color_t{181, 175, 174});

        game.simulate(input);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
