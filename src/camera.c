#include "camera.h"
#include "matrix_tools.h"
#include <math.h>

// TODO: debug only
#include <stdio.h>

const double scaling = 1;
const double D = 1; //0.01;

const double tan_half = 1; // for now, hard-code the half angle to be 45 deg
const double H = tan_half;
const double HalfWinWidth  = 0.5*SCREEN_WIDTH;
const double HalfWinHeight = 0.5*SCREEN_HEIGHT;

void screen_to_camera(double point[N_DIM], int screen_pos[2])
{
    // TODO: currently hardcoded
    // camera z pos. 
    point[3] = camera_z;
    point[0] = 0.0;
    point[1] = point[3] * H*(0.0 + screen_pos[0] - HalfWinWidth)/HalfWinWidth ;
    point[2] = point[3] * H*(0.0 + screen_pos[1] - HalfWinHeight)/HalfWinHeight ;
}

void screen_to_ray(double point[N_DIM], int screen_pos[2])
{
    screen_to_camera(point, screen_pos);
    point[2] = 1;
    vector_mult_const(point,    point, D);
    matrix_mult_pt(point,    view_inv, point);
}

void screen_to_coords(double point[N_DIM], int screen_pos[2]) {
    double p[N_DIM];
    screen_to_camera(p, screen_pos);

    matrix_mult_pt(p,    view_inv, p);

    point[0] = p[0];
    point[1] = sqrt(p[1]*p[1] + p[2]*p[2] + p[3]*p[3]);
    point[2] = M_PI / 2;
    point[3] = atan2(p[2], p[1]);
}

void coords_to_screen(int screen_pos[2], double point[N_DIM]) {
    double tmp[N_DIM];

    // TODO: fix later, for now assuming spherical coords
    double p[N_DIM];
    p[0] = point[0];
    p[1] = point[1] * sin(point[2]) * cos(point[3]);
    p[2] = point[1] * sin(point[2]) * sin(point[3]);
    p[3] = point[1] * cos(point[2]);

    matrix_mult_pt(tmp, view_mat, p);

    screen_pos[0] = (int) (tmp[1] / tmp[3] * HalfWinWidth   / H + HalfWinWidth);
    screen_pos[1] = (int) (tmp[2] / tmp[3] * HalfWinHeight  / H + HalfWinHeight);
}
