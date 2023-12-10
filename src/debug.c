#include "debug.h"
#include "camera.h"

Uint8 torgb(float color) {
    return (Uint8) (color * 255);
}

void debug_draw_point(double point[N_DIM],
                      float r,
                      float g,
                      float b,
                      float a)
{
    // get screen coords
    int s[2];
    coords_to_screen(s, point);

    // draw point
    SDL_SetRenderDrawColor(debug_renderer, torgb(r), torgb(g), torgb(b), torgb(a));
    SDL_RenderDrawPoint(debug_renderer, s[0], s[1]);
}

void debug_fill_circle(double point[N_DIM],
                       int radius,
                       float r,
                       float g,
                       float b,
                       float a
                      ) {

    // get screen coords
    int s[2];
    coords_to_screen(s, point);

    // fill circle
    SDL_SetRenderDrawColor(debug_renderer, torgb(r), torgb(g), torgb(b), torgb(a));
    for(int dx = -radius; dx <= radius; dx++) {
        for(int dy = -radius; dy <= radius; dy++) {
            if(dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(debug_renderer, s[0] + dx, s[1] + dy);
            }
        }
    }
}

void debug_draw_line(double point1[N_DIM],
                     double point2[N_DIM],
                     float r,
                     float g,
                     float b,
                     float a
                    ) {
    // get screen coords
    int s1[2];
    int s2[2];
    coords_to_screen(s1, point1);
    coords_to_screen(s2, point2);

    // draw line
    SDL_SetRenderDrawColor(debug_renderer, torgb(r), torgb(g), torgb(b), torgb(a));
    SDL_RenderDrawLine(debug_renderer, s1[0], s1[1], s2[0], s2[1]);
}
