#include "lightmodel.h"

#include "raymarcher.h"
#include "matrix_tools.h"
#include "debug.h"

#include <stdio.h>
#include <math.h>

double light_in_world_space[3] ;
int light_resolution_u           =  50 ;
int light_resolution_v           =  50 ;
int light_obj_res                =  50 ;
int light_interpolation_distance =   5 ;

void bake_light() {

    int onum, u, v, i;
    double tmp_baked_lights[num_objects][light_obj_res][light_obj_res][3];

    // init to 0
    for( onum=0; onum<num_objects; ++onum) {
        for( u=0; u<light_obj_res; ++u) {
            for( v=0; v<light_obj_res; ++v) {
                for( i=0; i<3; ++i) {
                    tmp_baked_lights[onum][u][v][i] = 0;
                }
            }
        }
    }

    // cast out a bunch of rays from the light source

    double u_d, v_d, Rtip[3];
    double point[3], V[3];

    int param_u, param_v;
    double param_point[2];

    // cast out rays in a sphere around light source
    for( u=0; u<light_resolution_u; ++u) {
        //for( u=0; u<50; ++u) {
        //for( v=0; v<1; ++v) {
        printf("starting u-cycle %d of %d\n", u, light_resolution_u);
        for( v=0; v<light_resolution_v; ++v) {

            u_d = u*2.0*M_PI / light_resolution_u ;
            v_d = v*2*M_PI / light_resolution_v - M_PI;
            Rtip[0] = cos(u_d)*cos(v_d) ;
            Rtip[1] = sin(u_d)*cos(v_d) ;
            Rtip[2] = sin(v_d) ;

            vector_add(Rtip, light_in_world_space, Rtip);

            onum = cast_ray(light_in_world_space, Rtip, point, V);
            if(onum == -1) {
                continue;
            }

            // move point to object space
            matrix_mult_pt(point, obinv[onum], point);

            // get parametric coords of point
            to_parametric[onum](point, param_point);
            param_u = param_point[0] * (light_obj_res-1);
            param_v = param_point[1] * (light_obj_res-1);

            vector_mult_const(tmp_baked_lights[onum][param_u][param_v], V, -1);

            {
                if(debug) {
                    // DEBUG
                    double p[3];
                    matrix_mult_pt(point, obmat[onum], point);
                    vector_mult_const(p, tmp_baked_lights[onum][param_u][param_v], 1);
                    vector_add(p, point, p);
                    debug_draw_point(p, 0, 1, 0, 1);
                }
            }

        }
    }

    // now interpolate
    // for each object, go through the entire parametrized space
    // and interpolate each value
    int interp_u, interp_v;
    double tmp[3];
    for( onum=0; onum<num_objects; ++onum) {
        for( u=0; u<light_obj_res; ++u) {
            for( v=0; v<light_obj_res; ++v) {

                // copy value over
                vector_copy(baked_lights[onum][u][v], tmp_baked_lights[onum][u][v]);

                // if light already hit this point, skip
                if(vector_magnitude(baked_lights[onum][u][v]) > 0) {
                    continue;
                }

                for(interp_u = u-light_interpolation_distance;
                        interp_u < u+light_interpolation_distance;
                        ++interp_u) {

                    if(interp_u < 0 || interp_u >= light_obj_res) {
                        continue;
                    }

                    for(interp_v = v-light_interpolation_distance;
                            interp_v < v+light_interpolation_distance;
                            ++interp_v) {

                        if(interp_v < 0 || interp_v >= light_obj_res) {
                            continue;
                        }

                        double dist = (interp_u - u)*(interp_u - u) + (interp_v - v)*(interp_v - v);
                        dist /= light_interpolation_distance*light_interpolation_distance;
                        vector_mult_const(tmp,
                                              tmp_baked_lights[onum][interp_u][interp_v],
                                              1.0 / (1.0 + dist));
                        vector_add(baked_lights[onum][u][v], baked_lights[onum][u][v], tmp);

                    }
                }

                const double mag = vector_magnitude(baked_lights[onum][u][v]);
                if(mag > 0) {
                    vector_mult_const(baked_lights[onum][u][v], baked_lights[onum][u][v], 1.0/mag);
                }
            }
        }
    }
}

// light model constants
double AMBIENT      = 0.2 ;
double MAX_DIFFUSE  = 0.5 ;
double SPECPOW      = 50 ;

int Light_Model (double irgb[3],
                 double E[3],
                 double N[3],
                 double P[2],
                 int onum,
                 double argb[3])
// s,p,n in eyespace

// irgb == inherent color of object (input to this function)
// E = vector from this poin to eye (input to this function)
// N = normal to the object at p (input to this function)
// P = parametric coordinates of point on object
// argb == actual color of object (output of this function)
// globals : AMBIENT, MAX_DIFFUSE, SPECPOW, light_in_eye_space[3]

// return 1 if successful, 0 if error
{
    // L = vector from this point to light
    const int param_x = P[0] * (light_obj_res-1);
    const int param_y = P[1] * (light_obj_res-1);
    double L[N_DIM];
    for(int i=0; i<N_DIM; ++i) {
        L[i] = baked_lights[onum][param_x][param_y][i];
    }


    // this needs to occur BEFORE you possibly jump to LLL below
    double max_ambient_and_diffuse = AMBIENT + MAX_DIFFUSE ;
    double intensity ;

    // normalize all vectors
    if(!vector_normalize(E, E)) {
        return 0;
    }
    if(!vector_normalize(N, N)) {
        return 0;
    }
    if(!vector_normalize(L, L)) {
        // if L has magnitude 0, then that means it's in shadow, so jump
        intensity = AMBIENT;
        goto LLL;
    }


    double NdotE = N[0]*E[0] + N[1]*E[1] + N[2]*E[2] ;
    double NdotL = N[0]*L[0] + N[1]*L[1] + N[2]*L[2] ;

    if (NdotL*NdotE < 0) {
        // eye and light are on opposite sides of polygon
        intensity = AMBIENT ;
        goto LLL ;
    } else if ((NdotL < 0) && (NdotE < 0)) {
        // eye and light on same side but normal pointing "wrong" way
        N[0] *= (-1.0) ;
        N[1] *= (-1.0) ;
        N[2] *= (-1.0) ;
        NdotE *= (-1.0) ;   // don't use NdotE below, probably should eliminate this
    }

    // ignore Blinn's variant
    double R[3] ; // Reflection vector of incoming light
    R[0] = 2*NdotL*N[0] - L[0] ;
    R[1] = 2*NdotL*N[1] - L[1] ;
    R[2] = 2*NdotL*N[2] - L[2] ;

    double EdotR = E[0]*R[0] + E[1]*R[1] + E[2]*R[2] ;

    double diffuse ;
    if (NdotL <= 0.0) {
        diffuse = 0.0 ;
    }
    else {
        diffuse = MAX_DIFFUSE*NdotL ;
    }

    double specular ;
    if (EdotR <= 0.0) {
        specular = 0.0 ;
    }
    else {
        specular = (1.0 - max_ambient_and_diffuse)*pow(EdotR,SPECPOW) ;
    }

    // printf("%lf %lf\n",diffuse,specular) ;
    intensity = AMBIENT + diffuse + specular ;

LLL :
    ;

    double f,g ;
    if (intensity <= max_ambient_and_diffuse) {
        f = intensity / max_ambient_and_diffuse ;
        argb[0] = f * irgb[0] ;
        argb[1] = f * irgb[1] ;
        argb[2] = f * irgb[2] ;
    } else {
        f = (intensity - max_ambient_and_diffuse) /
            (1.0 - max_ambient_and_diffuse) ;
        g = 1.0 - f ;
        argb[0] = g * irgb[0] + f ;
        argb[1] = g * irgb[1] + f ;
        argb[2] = g * irgb[2] + f ;
    }

    return 1 ;
}

