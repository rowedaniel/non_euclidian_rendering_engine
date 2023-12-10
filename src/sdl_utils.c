#include "sdl_utils.h"

int create_window(
    SDL_Window ** window,
    SDL_Renderer ** renderer,
    const char * title,
    int width,
    int height
) {
    // Create window
    (*window) = SDL_CreateWindow(title,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 width, height,
                                 SDL_WINDOW_SHOWN);
    if(!(*window)) {
        return 1;
    }

    // Create renderer
    (*renderer) = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(!(*renderer)) {
        SDL_DestroyWindow(*window);
        return 1;
    }

    return 0;
}
