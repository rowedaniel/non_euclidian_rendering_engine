#include "consts.h"
#include "object_support.h"

#include <stdbool.h>

bool debug;

// geometry info
void (*christoffel)(double point[N_DIM], double chris_sym[N_DIM][N_DIM][N_DIM]);

int cast_ray(double Rsource[N_DIM], double Rtip[N_DIM], double point[N_DIM], double V[N_DIM]);
int ray_to_rgb_recursive(double Rsource[N_DIM], double Rtip[N_DIM], double argb[N_DIM], bool do_lightmodel, int n);
