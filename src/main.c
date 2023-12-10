#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

#include "debug.h"
#include "sdl_utils.h"
#include "3d_objects.h"
#include "matrix_tools.h"
#include "raymarcher.h"
#include "camera.h"

// TODO: debug only
void print_christoffel(double chris_sym[N_DIM][N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        printf("christoffel[%d]\n", i);
        for(int j=0; j<N_DIM; ++j) {
            for(int k=0; k<N_DIM; ++k) {
                printf(" %12.4lf ", chris_sym[i][j][k]) ;
            }
            printf("\n");
        }
        printf("\n\n");
    }
}

/* // Define MAX and MIN macros */
/* #define MAX(X, Y) (((X) > (Y)) ? (X) : (Y)) */
/* #define MIN(X, Y) (((X) < (Y)) ? (X) : (Y)) */

void build_scene(double eye[N_DIM], double coi[N_DIM], double up[N_DIM]) {
    double Tvlist[100];
    int Tn, Ttypelist[100], Taxislist[100];
    double m[N_DIM+1][N_DIM+1], mi[N_DIM+1][N_DIM+1];

    num_objects = 0 ;
    color[num_objects][0] = 1.0 ;
    color[num_objects][1] = 1.0 ;
    color[num_objects][2] = 1.0 ;
    color_type[num_objects] = SIMPLE_COLOR;
    reflectivity[num_objects] = 0.0;
    Tn = 0;
    for(int axis=0; axis<N_DIM; ++axis) {
        Ttypelist[Tn] = SCALE;
        Taxislist[Tn] = axis;
        Tvlist[Tn] = 1;
        ++Tn;
    }
    matrix_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Taxislist, Tvlist);
    matrix_copy(obmat[num_objects], m);
    matrix_copy(obinv[num_objects], mi) ;
    SDF[num_objects] = sphere_SDF;
    grad[num_objects] = sphere_grad;
    to_parametric[num_objects] = sphere_to_parametric;

    draw[num_objects] = Draw_ellipsoid; // for 2d
    num_objects++ ; // don't forget to do this

    // place camera
    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 0;
    eye[3] = 20;

    coi[0] = 0;
    coi[1] = 0;
    coi[2] = 0;
    coi[3] = 0;

    up[0] = eye[0];
    up[1] = eye[1];
    up[2] = eye[2]+1;
    up[3] = eye[3];

    matrix_make_view(view_mat, view_inv, eye, coi, up);
    camera_z = eye[3];
}

const double schwarz_rad = 1;
void chris(double point[N_DIM], double chris_sym[N_DIM][N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            for(int k=0; k<N_DIM; ++k) {
                chris_sym[i][j][k] = 0;
            }
        }
    }

    const double max_val = 1e-6;

    if(point[1] != 0.0 && point[1] != schwarz_rad) {
        chris_sym[0][0][1] = schwarz_rad / (2*point[1]*(point[1] - schwarz_rad));
    } else {
        chris_sym[0][0][1] = max_val;
    }

    if(point[1] != 0.0) {
        chris_sym[1][0][0] = schwarz_rad / (2*point[1]*point[1]*point[1]);
    } else {
        chris_sym[1][0][0] = max_val;
    }
    chris_sym[1][1][1] = -chris_sym[0][0][1];
    chris_sym[1][2][2] = schwarz_rad - point[1];
    chris_sym[1][3][3] = (schwarz_rad - point[1]) * sin(point[2]) * sin(point[2]);

    if(point[1] != 0.0) {
        chris_sym[2][1][2] = 1 / point[1];
    } else {
        chris_sym[2][1][2] = max_val;
    }
    chris_sym[2][3][3] = -cos(point[2]) * sin(point[2]);

    chris_sym[3][1][3] = 1 / point[1];
    if(point[2] != 0.0) {
        chris_sym[3][2][3] = 1 / tan(point[2]);
    } else {
        chris_sym[3][2][3] = max_val;
    }

    for(int i=0; i<N_DIM; ++i) {
        for(int j=1; j<N_DIM; ++j) {
            for(int k=0; k<j; ++k) {
                chris_sym[i][j][k] = chris_sym[i][k][j];
            }
        }
    }
}

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if(create_window(&window, &renderer, "render", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return 1;
    }

    // init scene
    double eye[N_DIM], coi[N_DIM], up[N_DIM];
    printf("building scene\n");
    build_scene(eye, coi, up);
    printf("built scene\n");

    // define geometry
    christoffel = chris;

    // enable debug
    debug_renderer = renderer;

    double origin[N_DIM] = {0, 0, 0, 0};
    double Rsource[N_DIM] = {0, 6, M_PI/2, 0};
    double Rtip[N_DIM] = {0, 4, M_PI/2, M_PI/10};
    double point[N_DIM], V[N_DIM];

    {
        // DEBUG
        printf("ray tip in world space: \n");
        vector_print(Rtip);
        int s[2];
        coords_to_screen(s, Rtip);
        printf("ray point in camera space: \n");
        printf("Rtip: %d, %d\n", s[0], s[1]);

        screen_to_coords(point, s);
        printf("converted back to world space: \n");
        vector_print(point);
    }



    // main event loop
    while(1)
    {
        SDL_Event e;
        SDL_WaitEvent(&e);

        if(e.type == SDL_QUIT)
        {
            break;
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            printf("clicked at %d, %d\n", e.button.x, e.button.y);
            int p[2] = {e.button.x, e.button.y};
            double point[N_DIM];
            screen_to_coords(point, p);
            vector_copy(Rtip, point);
        }


        // clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        Draw_the_scene();
        debug_fill_circle(origin, 10, 0.2, 0.2, 0.2, 1);
        debug_fill_circle(Rsource, 5, 1, 0, 0, 1);
        debug_fill_circle(Rtip, 5, 0, 1, 0, 1);

        cast_ray(Rsource, Rtip, point, V);

        // update screen
        SDL_RenderPresent(renderer);
    }

    // clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
