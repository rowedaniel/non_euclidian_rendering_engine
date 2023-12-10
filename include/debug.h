#include <SDL.h>
#include "consts.h"

SDL_Renderer * debug_renderer;

void debug_draw_point(double point[N_DIM],
                      float r,
                      float g,
                      float b,
                      float a);


void debug_fill_circle(double point[N_DIM],
                       int radius,
                       float r,
                       float g,
                       float b,
                       float a
                      ) ;


void debug_draw_line(double point1[N_DIM],
                     double point2[N_DIM],
                     float r,
                     float g,
                     float b,
                     float a
                    ) ;

