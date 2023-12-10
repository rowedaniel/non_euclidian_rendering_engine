#include "consts.h"


// color
double color_type[M] ;
double color[M][3] ;
int image_IDs[M];
int image_size[M][2];
double reflectivity[M] ;

// object info
double obmat[M][N_DIM+1][N_DIM+1] ; // object space -> world space
double obinv[M][N_DIM+1][N_DIM+1] ; // world space -> object space
void (*grad[M])(double gradient[N_DIM], int onum, double intersection[N_DIM]); // used in normal calc
int (*to_parametric[M])(double point[N_DIM], double P[2]); // used in texture mapping
double (*SDF[M])(double point[N_DIM]); // used in raymarch collision detection
int obj_wormhole_side[M];
void (*draw[M])(int onum); 
double baked_lights[M][MAX_LIGHT_OBJ_RES][MAX_LIGHT_OBJ_RES][N_DIM];
int num_objects;
