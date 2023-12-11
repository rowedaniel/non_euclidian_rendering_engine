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
    //////////////////////////////////////////////////////////////
    const double dist = 4;
    const int gridSize = 10;
    for(int x=0; x<gridSize; ++x) {
      for(int y=1; y<gridSize; ++y) {
        color[num_objects][0] =       x * 1.0 / gridSize;
        color[num_objects][1] = 1.0 - x * 1.0 / gridSize;
        color[num_objects][2] =       y * 1.0 / gridSize;

        color[num_objects][x] = 0 ;
        color[num_objects][y] = 0.5 ;

        color_type[num_objects] = SIMPLE_COLOR;
        reflectivity[num_objects] = 0.0;
      
        Tn = 0 ;
        Ttypelist[Tn] = SCALE     ; Taxislist[Tn] = 1 ; Tvlist[Tn] =  1                    ; Tn++ ;
        Ttypelist[Tn] = SCALE     ; Taxislist[Tn] = 2 ; Tvlist[Tn] =  1                    ; Tn++ ;
        Ttypelist[Tn] = SCALE     ; Taxislist[Tn] = 3 ; Tvlist[Tn] =  1                    ; Tn++ ;
        Ttypelist[Tn] = TRANSLATE ; Taxislist[Tn] = 1 ; Tvlist[Tn] =  -1*dist              ; Tn++ ;
        Ttypelist[Tn] = TRANSLATE ; Taxislist[Tn] = 2 ; Tvlist[Tn] =  2.5 * (x-gridSize/2) ; Tn++ ;
        Ttypelist[Tn] = TRANSLATE ; Taxislist[Tn] = 2 ; Tvlist[Tn] =  2.5 * (y-gridSize/2) ; Tn++ ;
      
        matrix_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Taxislist, Tvlist);
        matrix_copy(obmat[num_objects], m);
        matrix_copy(obinv[num_objects], mi) ;
        obj_wormhole_side[num_objects] = 1;

        SDF[num_objects] = sphere_SDF;
        grad[num_objects] = sphere_grad;
        to_parametric[num_objects] = sphere_to_parametric;

        draw[num_objects] = Draw_ellipsoid; // for 2d
        num_objects++ ; // don't forget to do this
      }
    }
    //////////////////////////////////////////////////////////////

    // place camera
    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 0;
    eye[3] = 10;

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

const double wormhole_rad = 1;
void metric(double point[N_DIM], double metric[N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            metric[i][j] = 0;
        }
    }
    metric[0][0] = -1;
    metric[1][1] = 1;
    metric[2][2] = point[1]*point[1];
    metric[3][3] = point[1]*point[1]*sin(point[2])*sin(point[2]);
}

void ellis_chris(double point[N_DIM], double chris_sym[N_DIM][N_DIM][N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            for(int k=0; k<N_DIM; ++k) {
                chris_sym[i][j][k] = 0;
            }
        }
    }

    chris_sym[1][2][2] = -point[1];
    chris_sym[1][3][3] = -point[1]*sin(point[2])*sin(point[2]);

    chris_sym[2][1][2] = point[1] / (point[1]*point[1] + wormhole_rad*wormhole_rad);
    chris_sym[2][2][1] = chris_sym[2][1][2];
    chris_sym[2][3][3] = -cos(point[2])*sin(point[2]);

    chris_sym[3][1][3] = point[1] / (point[1]*point[1] + wormhole_rad*wormhole_rad);
    chris_sym[3][3][1] = chris_sym[3][1][3];
    chris_sym[3][2][3] = cos(point[2]) / sin(point[2]);
}




double dot_product(double a[N_DIM], double b[N_DIM], double point[N_DIM]) {
    double out = 0;
    double g[N_DIM][N_DIM];
    metric(point, g);

    for(int i=0; i<N_DIM; ++i) {
        for(int j=0; j<N_DIM; ++j) {
            out += g[i][j] * a[i] * b[j];
        }
    }
    return out;
}

int make_null_vector(double Rsource[N_DIM], double Rtip[N_DIM], double new_tip[N_DIM]) {
    // inputs:
    //  - point: point in coordinate space
    //  outputs:
    //  - V: null vector at point

    double V[N_DIM], time_vec[N_DIM];

    // build time vector
    for(int i=0; i<N_DIM; ++i) {
        time_vec[i] = 0;
    }
    time_vec[0] = 1;

    // build V
    vector_mult_const(V, Rsource, -1);
    vector_add(V, V, Rtip);

    double d = dot_product(V, V, Rsource);
    double d_time = dot_product(time_vec, time_vec, Rsource);
    double d_time_u = dot_product(time_vec, V, Rsource);

    const double det = d_time_u * d_time_u  - d_time * d;
    if(det < 0) {
        printf("null vector impossible\n");
        return 1;
    }
    const double t_component = -(d_time_u + sqrt(det)) / d_time;
    V[0] += t_component;

    vector_add(new_tip, Rsource, V);
    return 0;
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
    christoffel = ellis_chris;
    /* christoffel = minkowski_chris; */



    // enable debug
    debug_renderer = renderer;

    double origin[N_DIM] = {0, 0, 0, 0};
    double Rsource[N_DIM] = {0, 6, M_PI/2, 0};
    double Rtip[N_DIM] = {0, 6, M_PI/2, M_PI/10};
    double point[N_DIM], V[N_DIM];


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

            // check collision at clicked point
            double obj_point[N_DIM];
            vector_copy(obj_point, point);
            int dim = to_cartesian(obj_point, obj_point);
            if(obj_wormhole_side[0] == dim) {
                // transform point to object space
                matrix_mult_pt(obj_point, obinv[0], obj_point);
                double sdf = SDF[0](obj_point);
                if (sdf <= 0) {
                    printf("collision\n");
                }
            }


            make_null_vector(Rsource, point, Rtip);

            // get second deriv info
            double V[N_DIM];
            vector_mult_const(V, Rsource, -1);
            vector_add(V, V, Rtip);

            printf("point:\n");
            vector_print(point);
            printf("vector:\n");
            vector_print(V);
            printf("u.u:\n");
            printf("%f\n", dot_product(V, V, Rsource));
        }

        // clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        Draw_the_scene();

        cast_ray(Rsource, Rtip, point, V);
        double rgb[3];
        ray_to_rgb(Rsource, Rtip, false, rgb);

        debug_fill_circle(Rsource, 5, rgb[0], rgb[1], rgb[2], 1);
        debug_fill_circle(Rtip, 5, 0, 1, 0, 1);
        /* debug_fill_circle(origin, 5, 0.2, 0.2, 0.2, 1); */

        // update screen
        SDL_RenderPresent(renderer);
    }

    // clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
