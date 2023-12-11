#include "camera.h"
#include "matrix_tools.h"
#include <math.h>
#include <stdlib.h>

// TODO: debug only
#include <stdio.h>

const double scaling = 1;
const double D = 1; //0.01;

const double tan_half = 1; // for now, hard-code the half angle to be 45 deg
const double H = tan_half;
const double HalfWinWidth  = 0.5*SCREEN_WIDTH;
const double HalfWinHeight = 0.5*SCREEN_HEIGHT;

void to_spherical(double minkowski[N_DIM], int dim, double spherical[N_DIM]) {
    double s[N_DIM];
    s[0] = minkowski[0];
    s[1] = sqrt(minkowski[1]*minkowski[1] + minkowski[2]*minkowski[2] + minkowski[3]*minkowski[3]);
    if(s[1] == 0.0) {
        s[2] = 0.0;
        s[3] = 0.0;
        return;
    }
    s[2] = acos(minkowski[3] / s[1]);
    s[3] = atan2(minkowski[2], minkowski[1]);
    s[1] *= dim;
    vector_copy(spherical, s);
    return;
}

int to_cartesian(double spherical[N_DIM], double minkowski[N_DIM]) {
    double m[N_DIM];
    int dim = spherical[1] / ( fabs(spherical[1]));
    if(spherical[1] == 0.0) {
        dim = 1;
    }
    m[0] = spherical[0];
    m[1] = spherical[1] * sin(spherical[2]) * cos(spherical[3]);
    m[2] = spherical[1] * sin(spherical[2]) * sin(spherical[3]);
    m[3] = spherical[1] * cos(spherical[2]);
    m[1] *= dim;
    vector_copy(minkowski, m);
    return dim;
}

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

    to_spherical(p, 1, point);

    /* point[0] = p[0]; */
    /* point[1] = sqrt(p[1]*p[1] + p[2]*p[2] + p[3]*p[3]); */
    /* point[2] = M_PI / 2; */
    /* point[3] = atan2(p[2], p[1]); */
}

void coords_to_screen(int screen_pos[2], double point[N_DIM]) {
    double tmp[N_DIM];

    // TODO: fix later, for now assuming spherical coords
    double p[N_DIM];
    /* p[0] = point[0]; */
    /* p[1] = point[1] * sin(point[2]) * cos(point[3]); */
    /* p[2] = point[1] * sin(point[2]) * sin(point[3]); */
    /* p[3] = point[1] * cos(point[2]); */
    to_cartesian(point, p);
    matrix_mult_pt(tmp, view_mat, p);
    screen_pos[0] = (int) (tmp[1] / tmp[3] * HalfWinWidth   / H + HalfWinWidth);
    screen_pos[1] = (int) (tmp[2] / tmp[3] * HalfWinHeight  / H + HalfWinHeight);
}
