#include "object_support.h"

#include <SDL.h>


void Draw_ellipsoid (int onum);
void Draw_plane (int onum);
void Draw_hyperbola(int onum);
void Draw_the_scene();

double sphere_SDF(double point[N_DIM]);
double inv_sphere_SDF(double point[N_DIM]);
void sphere_grad(double gradient[N_DIM], int onum, double intersection[N_DIM]);
int sphere_to_parametric(double point[N_DIM], double P[2]);
