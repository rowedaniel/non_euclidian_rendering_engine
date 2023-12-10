#define SCALE 0
#define ROTATE 1
#define TRANSLATE 2

#include "consts.h"


int matrix_print (double a[N_DIM+1][N_DIM+1]);


int matrix_copy (double a[N_DIM+1][N_DIM+1], double b[N_DIM+1][N_DIM+1]);


int matrix_mult (double res[N_DIM+1][N_DIM+1], double a[N_DIM+1][N_DIM+1], double b[N_DIM+1][N_DIM+1]);


int matrix_mult_pt (double P[N_DIM],   double m[N_DIM+1][N_DIM+1], double Q[N_DIM]);


int matrix_x_product (double res[N_DIM], double a[N_DIM], double b[N_DIM]);


void vector_print(double a[]);


void vector_copy(double a[N_DIM], double b[N_DIM]) ;


double vector_dot_product(double a[N_DIM], double b[N_DIM]) ;


double vector_magnitude(double a[N_DIM]) ;


void vector_mult_const(double res[N_DIM], double a[N_DIM], double b) ;


void vector_add(double res[N_DIM], double a[N_DIM], double b[N_DIM]) ;


int vector_normalize(double res[N_DIM], double a[N_DIM]) ;


void matrix_transpose(double a[N_DIM+1][N_DIM+1], double b[N_DIM+1][N_DIM+1]);


int matrix_make_identity (double I[N_DIM+1][N_DIM+1]);


int matrix_make_translation (double a[N_DIM+1][N_DIM+1], double dP[N_DIM]);


int matrix_make_scaling (double a[N_DIM+1][N_DIM+1], double sP[N_DIM]);

int matrix_make_rotation_cs (double a[N_DIM+1][N_DIM+1], int axis1, int axis2, double cs, double sn);


void matrix_make_movement_sequence_matrix(double v[N_DIM+1][N_DIM+1], double vi[N_DIM+1][N_DIM+1],
        int n, int mtype[], int maxis[], double mparam[]);


void matrix_make_view(double v[N_DIM+1][N_DIM+1], double vi[N_DIM+1][N_DIM+1],  double eyeA[N_DIM], double coiA[N_DIM], double upA[N_DIM]);


