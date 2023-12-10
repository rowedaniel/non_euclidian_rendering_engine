#include "3d_objects.h"
#include "camera.h"
#include "matrix_tools.h"
#include "debug.h"
#include "object_support.h"

#include <math.h>
#include <SDL.h>

// ================ Sphere stuff ===============
double sphere_SDF(double point[N_DIM]) {
    // spherical coords:
    return point[1] -1;
    // x,y,z
    /* // f(x,y,z) = x^2 + y^2 + z^2 - 1 */
    /* return vector_dot_product(point, point) - 1; */
}

double inv_sphere_SDF(double point[N_DIM]) {
    // inverted sphere where in is out and out is in
    return -sphere_SDF(point);
}

void sphere_grad(double gradient[N_DIM], int onum, double intersection[N_DIM]) {
    // for spheres, gradient is <2x, 2y, 2z>
    vector_mult_const(gradient, intersection, 2);
}

int sphere_to_parametric(double point[N_DIM], double P[2])
{
    // parameterization = (cos(u)cos(v), sin(u)cos(v), sin(v))
    // so v=asin(z), u=atan2(y,x)
    // finally rescale so that 0<=u,v<=1

    P[1] = point[1];
    if(point[1] <= -1 || point[1] >= 1) {
        P[1] = (P[1] + 1) / 2;
        P[0] = 0;
        return 0;
    }
    P[0] = (atan2(point[2]/sqrt(1-P[1]*P[1]), point[0]/sqrt(1-P[1]*P[1])) + M_PI) / M_PI / 2;
    P[1] = (P[1] + 1) / 2;
    return 1;
}

// ==========================================================================

// ================ plane stuff ===============
double plane_SDF(double point[N_DIM]) {
    if(-1 > point[0] || point[0] > 1 ||
            -1 > point[1] || point[1] > 1) {
        return M_HITHER;
    }
    return point[2]*point[2];
}

void plane_grad(double gradient[N_DIM], int onum, double intersection[N_DIM]) {
    gradient[0] = 0;
    gradient[1] = 0;
    gradient[2] = 1;
}
int plane_to_parametric(double point[N_DIM], double P[2])
{
    // plane parameterization is (u, v, 0)
    // rescale so that 0<=u,v<=1
    P[0] = (point[0] + 1) / 2;
    P[1] = (point[1] + 1) / 2;
    return 1;
}
// ==========================================================================

// ================ triangle stuff ===============
void triangle_grad(double gradient[N_DIM], int onum, double intersection[N_DIM]) {
    gradient[0] = 0;
    gradient[1] = 0;
    gradient[2] = 1;
}
int triangle_to_parametric(double point[N_DIM], double P[2])
{
    // triangle parameterization is (u, v, 0)
    // rescale so that 0<=u,v<=1
    P[0] = point[0];
    P[1] = point[1];
    return 1;
}
// ==========================================================================

// ================ hyperboloid stuff ===============
void hyperboloid_grad(double gradient[N_DIM], int onum, double intersection[N_DIM]) {
    // for hyperboloids, gradient is <2x, -2y, 2z>
    matrix_mult_pt(gradient, obinv[onum], intersection);
    vector_mult_const(gradient, gradient, 2);
    gradient[1] *= -1;
}

int hyperboloid_to_parametric(double point[N_DIM], double P[2])
{
    // hyperboloid parameterization is (cos(u)sec(v), sin(u)sec(v), tan(v))
    // inverse is u=atan2(z,x), v=atan(y)
    // rescale so that 0<=u,v<=1
    P[0] = (atan2(point[2], point[0]) + M_PI) / M_PI / 2;
    P[1] = (-atan(point[1]) + M_PI / 2) / M_PI;
    return 1;
}
// ==========================================================================
//
void Draw_ellipsoid (int onum)
{
    int n,i ;
    double t, xyz[N_DIM] ;

    // spherical
    const double theta = M_PI / 2;
    const double r = 1;
    xyz[0] = 0;
    xyz[1] = r;
    xyz[2] = theta;
    for(double phi=0; phi < 2*M_PI; phi += M_PI/400) {
        xyz[3] = phi;
        matrix_mult_pt(xyz, obmat[onum], xyz);
        debug_draw_point(xyz, color[onum][0],color[onum][1],color[onum][2], 1);
    }

    // Euclidean
    /* n = 10000 ; */
    /* for (i = 0 ; i < n ; i++) { */
    /*     t = i*2*M_PI/n ; */
    /*     xyz[0] = cos(t) ; */
    /*     xyz[1] = sin(t) ; */
    /*     xyz[2] = 0 ; */
    /*     matrix_mult_pt(xyz, obmat[onum], xyz) ; */

    /*     debug_draw_point(xyz, color[onum][0],color[onum][1],color[onum][2], 1); */
    /* } */
}

void Draw_plane (int onum)
{
    int n,i ;
    double t, xyz[N_DIM] ;

    n = 1000 ;
    for (i = 0 ; i < n ; i++) {
        t = (i * 1.0 /  n  - 0.5) * 2;
        xyz[0] = t ;
        xyz[1] = 0 ;
        xyz[2] = 0 ;
        debug_draw_point(xyz, color[onum][0],color[onum][1],color[onum][2], 1);
    }
}

void Draw_hyperbola(int onum)
{
    int n,i ;
    double t, xyz[N_DIM] ;

    n = 1000 ;
    for (i = 0 ; i < n ; i++) {
        t = i * 2 * M_PI / n - M_PI;
        xyz[0] = 1/cos(t) ;
        xyz[1] = tan(t)   ;
        xyz[2] = 0 ;
        if(-1 > xyz[1] || xyz[1] > 1) {
            continue;
        }
        matrix_mult_pt(xyz, obmat[onum], xyz) ;
    }
}
// ==========================================================================

void Draw_the_scene()
{
    int onum ;
    for (onum = 0 ; onum < num_objects ; onum++) {
        draw[onum](onum) ;
    }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
