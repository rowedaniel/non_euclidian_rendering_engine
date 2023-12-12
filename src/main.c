#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include <SDL.h>

#include "debug.h"
#include "sdl_utils.h"
#include "3d_objects.h"
#include "matrix_tools.h"
#include "raymarcher.h"
#include "camera.h"

void build_scene(double eye[N_DIM], double coi[N_DIM], double up[N_DIM]) {
    double Tvlist[100];
    int Tn, Ttypelist[100], Taxislist[100];
    double m[N_DIM+1][N_DIM+1], mi[N_DIM+1][N_DIM+1];

    num_objects = 0 ;
    //////////////////////////////////////////////////////////////
    const double dist = 30;
    const int gridSize = 30;
    for(int dim=-1; dim<2; dim+=2) {
        for(int x=0; x<gridSize; ++x) {
            for(int y=0; y<gridSize; ++y) {
                if(dim == 1) {
                    color[num_objects][0] =       x * 1.0 / gridSize;
                    color[num_objects][1] = 1.0 - x * 1.0 / gridSize;
                    color[num_objects][2] =       y * 1.0 / gridSize;
                } else {
                    color[num_objects][0] = 1.0 - x * 1.0 / gridSize;
                    color[num_objects][1] =       x * 1.0 / gridSize;
                    color[num_objects][2] = 1.0 - y * 1.0 / gridSize;
                }

                color_type[num_objects] = SIMPLE_COLOR;
                reflectivity[num_objects] = 0.0;

                Tn = 0 ;
                Ttypelist[Tn] = SCALE     ;
                Taxislist[Tn] = 1 ;
                Tvlist[Tn] =  1                    ;
                Tn++ ;
                Ttypelist[Tn] = SCALE     ;
                Taxislist[Tn] = 2 ;
                Tvlist[Tn] =  1                    ;
                Tn++ ;
                Ttypelist[Tn] = SCALE     ;
                Taxislist[Tn] = 3 ;
                Tvlist[Tn] =  1                    ;
                Tn++ ;
                Ttypelist[Tn] = TRANSLATE ;
                Taxislist[Tn] = 1 ;
                Tvlist[Tn] =  2.5 * (y-gridSize/2) ;
                Tn++ ;
                Ttypelist[Tn] = TRANSLATE ;
                Taxislist[Tn] = 2 ;
                Tvlist[Tn] =  2.5 * (x-gridSize/2) ;
                Tn++ ;
                Ttypelist[Tn] = TRANSLATE ;
                Taxislist[Tn] = 3 ;
                Tvlist[Tn] =   1*dist              ;
                Tn++ ;

                matrix_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Taxislist, Tvlist);
                matrix_copy(obmat[num_objects], m);
                matrix_copy(obinv[num_objects], mi) ;
                obj_wormhole_side[num_objects] = dim;

                SDF[num_objects] = sphere_SDF;
                grad[num_objects] = sphere_grad;
                to_parametric[num_objects] = sphere_to_parametric;

                draw[num_objects] = Draw_ellipsoid; // for 2d
                num_objects++ ; // don't forget to do this
            }
        }
    }
    
    // make sphere to prevent rays from going too far
    color[num_objects][0] = 1.0;
    color[num_objects][1] = 1.0;
    color[num_objects][2] = 1.0;

    color_type[num_objects] = SIMPLE_COLOR;
    reflectivity[num_objects] = 0.0;

    Tn = 0 ;
    Ttypelist[Tn] = SCALE     ;
    Taxislist[Tn] = 1 ;
    Tvlist[Tn] =  50                    ;
    Tn++ ;
    Ttypelist[Tn] = SCALE     ;
    Taxislist[Tn] = 2 ;
    Tvlist[Tn] =  50                    ;
    Tn++ ;
    Ttypelist[Tn] = SCALE     ;
    Taxislist[Tn] = 3 ;
    Tvlist[Tn] =  50                    ;
    Tn++ ;

    matrix_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Taxislist, Tvlist);
    matrix_copy(obmat[num_objects], m);
    matrix_copy(obinv[num_objects], mi) ;
    obj_wormhole_side[num_objects] = 1;

    SDF[num_objects] = inv_sphere_SDF;
    grad[num_objects] = sphere_grad;
    to_parametric[num_objects] = sphere_to_parametric;

    draw[num_objects] = Draw_ellipsoid; // for 2d
    num_objects++ ; // don't forget to do this
    //////////////////////////////////////////////////////////////

    // place camera
    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 0;
    eye[3] = -10;

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


Uint8 all_points[SCREEN_WIDTH][SCREEN_HEIGHT][3];
void set_point_color(double inrgb[3], int i, int j) {
    all_points[i][j][0] = (Uint8) (inrgb[0] * 255);
    all_points[i][j][1] = (Uint8) (inrgb[1] * 255);
    all_points[i][j][2] = (Uint8) (inrgb[2] * 255);
}
void draw_all_points(SDL_Renderer *renderer) {
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0x00, 0xF0, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    for(int i=0; i<SCREEN_WIDTH; ++i) {
        for(int j=0; j<SCREEN_HEIGHT; ++j) {
            SDL_SetRenderDrawColor(renderer, all_points[i][j][0], all_points[i][j][1], all_points[i][j][2], 0xFF);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }

    // update screen
    SDL_RenderPresent(renderer);
}

void render_pixel(double eye[N_DIM], double Rsource[3], int i, int j) {
    double look[N_DIM] = {0, (2.0 * i / SCREEN_WIDTH) - 1.0, (2.0 * j / SCREEN_HEIGHT) - 1.0, 1};
    double Rtip[N_DIM];
    double rgb[3];

    vector_add(look, look, eye);
    to_spherical(look, 1, look);
    make_null_vector(Rsource, look, Rtip);

    ray_to_rgb(Rsource, Rtip, false, rgb);
    set_point_color(rgb, i, j);

}


struct render_args {
    int min_i;
    int max_i;
    int min_j;
    int max_j;
    double Rsource[N_DIM];
    double eye[N_DIM];
};

void *render_grid(void *vargp) {
    struct render_args *args = (struct render_args *) vargp;
    int min_i = args->min_i;
    int max_i = args->max_i;
    int min_j = args->min_j;
    int max_j = args->max_j;
    double *Rsource, *eye;
    Rsource = args->Rsource;
    eye = args->eye;

    printf("rendering %d %d %d %d\n", min_i, max_i, min_j, max_j);

    const int res = 1;
    for(int i=min_i; i<max_i; i+=res) {
        for(int j=min_j; j<max_j; j+=res) {
            render_pixel(eye, Rsource, i, j);
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
    christoffel = ellis_chris;

    // enable debug
    debug = false;

    // initial render
    for(int i=0; i<SCREEN_WIDTH; ++i) {
        for(int j=0; j<SCREEN_HEIGHT; ++j) {
            all_points[i][j][0] = 0;
            all_points[i][j][1] = 0;
            all_points[i][j][2] = 0;
        }
    }
    double Rsource[N_DIM] = {0, 6, M_PI/2, 0};
    double Rtip[N_DIM] = {0, 6, M_PI/2, M_PI/10};
    double rgb[3];
    to_spherical(eye, 1, Rsource);



    const int n_threads = SCREEN_WIDTH/16;
    pthread_t thread[n_threads];
    printf("threads allotted: %d\n", n_threads);
    struct render_args args[n_threads];
    const int res = SCREEN_WIDTH/2/n_threads;
    for(int i=2; i<n_threads; ++i) {
        printf("doing %d to %d\n", i*res, (i+1)*res);
    /* for(int i=300; i<500; i+=res) { */
        args[i].min_i = i*res;
        args[i].max_i = (i+1)*res;
        /* args[i].min_j = 375; */
        /* args[i].max_j = 425; */
        args[i].min_j = 16;
        args[i].max_j = SCREEN_HEIGHT/2;
        vector_copy(args[i].Rsource, Rsource);
        vector_copy(args[i].eye, eye);

        pthread_create(&thread[i], NULL, render_grid, (void *) &args[i]);
    }

    int next_free_thread = 0;

    // main event loop
    while(1)
    {

        SDL_Event e;
        SDL_WaitEvent(&e);

        if(e.type == SDL_QUIT)
        {
            break;
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            /* printf("clicked at %d, %d\n", e.button.x, e.button.y); */
            /* const int i = e.button.x; */
            /* const int j = e.button.y; */

            /* pthread_join(thread[next_free_thread], NULL); */
            /* args[next_free_thread].min_i = i-5; */
            /* args[next_free_thread].max_i = i+5; */
            /* args[next_free_thread].min_j = j-5; */
            /* args[next_free_thread].max_j = j+5; */
            /* vector_copy(args[next_free_thread].Rsource, Rsource); */
            /* vector_copy(args[next_free_thread].eye, eye); */

            /* pthread_create(&thread[next_free_thread], NULL, render_grid, (void *) &args[next_free_thread]); */
            /* next_free_thread = (next_free_thread + 1) % SCREEN_WIDTH; */

            /* render_pixel(eye, Rsource, i, j); */
        }
        draw_all_points(renderer);
    }

    for(int i=0; i<n_threads; ++i) {
        pthread_join(thread[i], NULL);
    }

    // clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // write image to file
    FILE *f = fopen("image.ppm", "w");
    fprintf(f, "P3\n%d %d\n%d\n", SCREEN_WIDTH, SCREEN_HEIGHT, 255);
    for(int j=SCREEN_HEIGHT-1; j>=0; --j) {
        for(int i=0; i<SCREEN_WIDTH; ++i) {
            fprintf(f, "%d %d %d ", all_points[i][j][0], all_points[i][j][1], all_points[i][j][2]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    return 0;
}
