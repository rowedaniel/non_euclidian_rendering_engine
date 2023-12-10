#include "consts.h"

void screen_to_camera(double point[N_DIM], int screen_pos[2]);
void screen_to_ray(double point[N_DIM], int screen_pos[2]);
void screen_to_coords(double point[N_DIM], int screen_pos[2]);
void coords_to_screen(int screen_pos[2], double point[N_DIM]);
double view_mat[N_DIM+1][N_DIM+1], view_inv[N_DIM+1][N_DIM+1];
double camera_z;
