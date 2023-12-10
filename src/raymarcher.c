
#include "raymarcher.h"
#include "matrix_tools.h"
#include "debug.h"
#include "camera.h"
#include "lightmodel.h"

#include <math.h>

int wormhole_side = 1;

void path_second_deriv(double point[N_DIM], double V[N_DIM], double second_deriv[N_DIM]) {
    // inputs:
    //  - point: point in coordinate space
    //  - V: velocity vector at point
    //  outputs:
    //  - second_deriv: second derivative of path at point in direction of V

    double chris_sym[N_DIM][N_DIM][N_DIM];
    christoffel(point, chris_sym);


    for(int i=0; i<N_DIM; ++i) {
        second_deriv[i] = 0;
        for(int j=0; j<N_DIM; ++j) {
            for(int k=0; k<N_DIM; ++k) {
                second_deriv[i] += chris_sym[i][j][k] * V[j] * V[k];
            }
        }
    }


}

// gets an object's base color (prior to light model)
int get_color(int onum, double P[2], double rgb[3])
{
    if(color_type[onum] == SIMPLE_COLOR) {
        for(int i=0; i<3; ++i) {
            rgb[i] = color[onum][i];
        }
    } else if(color_type[onum] == TEXTURE_COLOR) {
        // convert to object space
        int x = floor( image_size[onum][0] * P[0] );
        int y = floor( image_size[onum][1] * P[1] );
        // TODO: figure out reading color from texture
        /* int e = get_xwd_map_color(image_IDs[onum], x,y,rgb); */
        /* return e; */
    }
    return 0;
}

double delta_t = 0.0001;
double max_ray_distance = 100000;

int cast_ray(double Rsource[N_DIM], double Rtip[N_DIM], double point[N_DIM], double V[N_DIM])
// casts a ray out from Rsource to Rtip, and checks if it intersects with any object.
// Returns:
//   - point[N_DIM] - point of intersection with object (typically slightly moved away from object)
//   - V[N_DIM]     - direction the ray came from
//   - onum     - the object number that was hit (or -1 if nothing was hit)
// (though because the geometry is non-Euclidean, will probably not reach Rtip)
//
{
    double dV_dt[N_DIM], next_point[N_DIM], obj_point[N_DIM];
    int onum;

    // V = Rtip - Rsource
    vector_mult_const(V, Rsource, -1);
    vector_add(V, Rtip, V);
    // currently, normalize V. TODO: should I keep doing this?
    vector_normalize(V, V);

    vector_copy(next_point, Rsource);

    for(int n=0; n < max_ray_distance; ++n) {

        if(vector_magnitude(Rsource) > M_YON) {
            return -1;
        }

        /* // TODO: currently hardcoding spherical coords. Fix this later. */
        /* // note: convention used here is physics, where */
        /* // - theta is the angle from the z-axis */
        /* // - phi is the angle from the x-axis in the xy-plane */
        /* double x_prime[N_DIM]; */
        /* x_prime[0] = point[0]; */
        /* x_prime[1] = wormhole_side * sqrt(point[1]*point[1] + point[2]*point[2] + point[3]*point[3]); */
        /* x_prime[2] = acos(point[3] / r); */
        /* x_prime[3] = atan2(point[2], point[1]); */

        // get next point in the ray
        //
        // update point to be the old next_point, then
        // calculate new next_point and update V
        vector_copy(point, next_point);
        // next point = point + (V * delta_t)
        vector_mult_const(next_point, V, delta_t);
        vector_add(next_point, next_point, point);
        // next V = V + dV/dt * delta_t
        path_second_deriv(next_point, V,   dV_dt);
        vector_mult_const(dV_dt,   dV_dt, delta_t);
        vector_add(V,   V, dV_dt);

        {
            // DEBUG
            debug_draw_point(point, 1, 1, 1, 1);
        }

        // check for collision with any object
        for(onum=0; onum < num_objects; ++onum) {
            // transform point to object space
            matrix_mult_pt(obj_point, obinv[onum], next_point);

            double sdf = SDF[onum](obj_point);
            if (sdf <= 0) {
                return onum;
            }
        }

    }

    return -1;
}

void get_normal(double normal[N_DIM], int onum, double intersection[N_DIM]) {
    // get gradient
    double gradient[N_DIM];
    double intersection_obj[N_DIM];
    matrix_mult_pt(intersection_obj, obinv[onum], intersection);
    grad[onum](gradient, onum, intersection_obj);

    // finally, transform back into object space
    double inv_T[N_DIM+1][N_DIM+1];
    matrix_transpose(inv_T, obinv[onum]);
    matrix_mult_pt(normal, inv_T, gradient);

    vector_normalize(normal, normal);
}

int ray_to_rgb_recursive(double Rsource[N_DIM], double Rtip[N_DIM], double argb[N_DIM], bool do_lightmodel, int n)
{
    // default color to black
    for(int j=0; j<3; ++j) {
        argb[j] = 0;
    }

    // get point of intersection and look vector
    double point[N_DIM], look[N_DIM];
    int saved_onum = cast_ray(Rsource, Rtip, point, look);
    if(saved_onum == -1) {
        // no intersection
        return 0;
    }

    // set color to this object's color:
    double o_color[N_DIM];
    get_color(saved_onum, point, o_color);

    // calculate normal
    double normal[N_DIM];
    get_normal(normal, saved_onum, point);

    if(do_lightmodel) {

        // flip look vector so it points TO eye FROM point
        vector_mult_const(look, look, -1);

        // get parametrized coordinates
        double o_point[N_DIM], P[2];
        matrix_mult_pt(o_point, obinv[saved_onum], point);
        to_parametric[saved_onum](o_point, P);

        // DEBUG: draw point
        if(debug) {
            const int param_x = P[0] * (50-1);
            const int param_y = P[1] * (50-1);

            double p[N_DIM];
            const int x = param_x;
            const int y = param_y;
            const double len = 1;

            /*
            // normal vector
            vector_mult_const(p, normal, len);
            vector_add(p, point, p);
            debug_draw_line(point, p, 0.5, 0.5, 0, 1);
            */

            // light vector
            vector_mult_const(p, baked_lights[saved_onum][x][y], len);
            vector_add(p, point, p);
            debug_draw_line(point, p, 0, 0.8, 0.5, 1);

            /*
            // look vector
            vector_mult_const(p, look, len);
            vector_add(p, point, p);
            debug_draw_line(point, p, 0, 1, 0, 1);
            */
        }

        Light_Model(o_color,
                    look,
                    normal,
                    P,
                    saved_onum,
                    o_color);
    }

    if(debug) {
        debug_draw_point(point, o_color[0], o_color[1], o_color[2], 1);
    }

    double new_color[3] = {0, 0, 0};

    // recurse!
    if (n > 0 && reflectivity[saved_onum] > 0) {

        // reflection = look - 2(look * normal)normal
        double reflection[N_DIM];
        vector_mult_const(reflection, normal, -2*vector_dot_product(look, normal));
        vector_add(reflection, reflection, look);

        // move point out slightly, to avoid colliding with the same object again
        vector_mult_const(reflection, reflection, M_HITHER);
        vector_add(point, point, reflection);

        // new tip = reflection + intersection
        double new_tip[N_DIM];
        vector_add(new_tip, reflection, point);

        if(debug) {
            // DEBUG
            double debug_tip[N_DIM];
            vector_mult_const(debug_tip, reflection, 1);
            vector_add(debug_tip, debug_tip, point);

            debug_draw_line(point, debug_tip, o_color[0], o_color[1], o_color[2], 1);
            debug_fill_circle(point, 4, o_color[0], o_color[1], o_color[2], 1);
        }

        ray_to_rgb_recursive(point, new_tip, new_color, do_lightmodel, n-1);

    }

    for(int j=0; j<3; ++j) {
        const double r = reflectivity[saved_onum];
        argb[j] = (1-r) * o_color[j] + r * new_color[j];
    }

    return 1;
}

void ray_to_rgb(double Rsource[N_DIM], double Rtip[N_DIM], bool do_lightmodel, double argb[3])
{
    ray_to_rgb_recursive(Rsource, Rtip, argb, do_lightmodel, 0);
}
