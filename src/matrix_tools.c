
#include "matrix_tools.h"

#include <stdio.h>
#include <math.h>

/*

 ( w')          (w)
 ( x')          (x)
 ( y')  =   M * (y)
 ( z')          (z)
 ( 1 )          (1)

instead of (w',x',y',z',1) = (w,x,y,z,1) * M

*/

int matrix_print (double a[N_DIM+1][N_DIM+1])
{
    int r,c;
    for (r = 0; r < N_DIM+1; r++ ) {
        for (c = 0; c < N_DIM+1; c++ ) {
            printf(" %12.4lf ",a[r][c]);
        }
        printf("\n");
    }

    return 1;
}

int matrix_copy (double a[N_DIM+1][N_DIM+1], double b[N_DIM+1][N_DIM+1])
// a = b
{
    int r,c;
    for (r = 0; r < N_DIM+1; r++ ) {
        for (c = 0; c < N_DIM+1; c++ ) {
            a[r][c] = b[r][c];
        }
    }

    return 1;
}

int matrix_mult (double res[N_DIM+1][N_DIM+1], double a[N_DIM+1][N_DIM+1], double b[N_DIM+1][N_DIM+1])
// res = a * b
// this is SAFE, i.e. the user can make a call such as
// matrix_mult(p,  p,q) or matrix_mult(p,  q,p) or  matrix_mult(p, p,p)
{
    double sum;
    int k;
    int r,c;
    double tmp[N_DIM+1][N_DIM+1];

    for (r = 0; r < N_DIM+1; r++ ) {
        for (c = 0; c < N_DIM+1; c++ ) {
            sum = 0.0;
            for (k = 0; k < N_DIM+1; k++) {
                sum = sum + a[r][k]*b[k][c];
            }
            tmp[r][c] = sum;
        }
    }

    matrix_copy(res,tmp);

    return 1;
}

int matrix_mult_pt (double P[N_DIM],   double m[N_DIM+1][N_DIM+1], double Q[N_DIM])
// P = m*Q
// SAFE, user may make a call like matrix_mult_pt (W, m,W);
{
    double out[N_DIM];
    double sum;
    int r,c;

    for (r = 0; r < N_DIM; r++ ) {
        sum = 0.0;
        for (c = 0; c < N_DIM; c++ ) {
            sum += m[r][c] * Q[c];
        }
        out[r] = sum + m[r][N_DIM];
    }

    vector_copy(P,out);
    return 1;
}


int matrix_x_product (double res[3], double a[3], double b[3])
// res = a x b  , cross product of two vectors
// SAFE: it is ok to make a call such as
// D3d_x_product (a,  a,b) or
// D3d_x_product (b,  a,b) or
// D3d_x_product (a,  a,a)
{
    double r[3];
    int v;

    r[0] = a[1]*b[2] - b[1]*a[2];
    r[1] = b[0]*a[2] - a[0]*b[2];
    r[2] = a[0]*b[1] - b[0]*a[1];

    res[0] = r[0];
    res[1] = r[1];
    res[2] = r[2];

    if ((res[0] == 0) && (res[1] == 0) && (res[2] == 0)) {
        v = 0;
    } else {
        v = 1;
    }

    return v;
}

void vector_print(double a[N_DIM])
{
    for(int i=0; i<N_DIM; i++) {
        printf("%lf ", a[i]);
    }
    printf("\n");
}

void vector_copy(double a[N_DIM], double b[N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        a[i] = b[i];
    }
}

double vector_dot_product(double a[N_DIM], double b[N_DIM]) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

double vector_magnitude(double a[N_DIM]) {
    return sqrt(vector_dot_product(a, a));
}

void vector_mult_const(double res[N_DIM], double a[N_DIM], double b) {
    for(int i=0; i<N_DIM; ++i) {
        res[i] = a[i]*b;
    }
}

void vector_add(double res[N_DIM], double a[N_DIM], double b[N_DIM]) {
    for(int i=0; i<N_DIM; ++i) {
        res[i] = a[i]+b[i];
    }
}

int vector_normalize(double res[N_DIM], double a[N_DIM]) {
    double magnitude = vector_magnitude(a);
    if(magnitude == 0) {
        return 0;
    }
    vector_mult_const(res, a, 1/magnitude);
    return 1;
}

void matrix_transpose(double a[N_DIM+1][N_DIM+1], double b[N_DIM+1][N_DIM+1])
{
    double tmp[N_DIM+1][N_DIM+1];
    matrix_copy(tmp, b);

    for(int i=0; i<N_DIM+1; ++i) {
        for(int j=0; j<N_DIM+1; ++j) {
            a[j][i] = tmp[i][j];
        }
    }
}


// creating matrices
int matrix_make_identity (double I[N_DIM+1][N_DIM+1])
{
    int r,c;
    for (r = 0; r < N_DIM+1; r++ ) {
        for (c = 0; c < N_DIM+1; c++ ) {
            if (r == c) I[r][c] = 1.0;
            else    I[r][c] = 0.0;
        }
    }

    return 1;
}

int matrix_make_translation (double a[N_DIM+1][N_DIM+1], double dP[N_DIM])
{
    matrix_make_identity(a);
    for(int i = 0; i < N_DIM; i++) {
        a[i][N_DIM] = dP[i];
    }
    return 1;
}

int matrix_make_scaling (double a[N_DIM+1][N_DIM+1], double sP[N_DIM])
{
    matrix_make_identity(a);
    for(int i = 0; i < N_DIM; i++) {
        a[i][i] = sP[i];
    }
    return 1;
}

int matrix_make_rotation_cs (double a[N_DIM+1][N_DIM+1], int axis1, int axis2, double cs, double sn)
{
    matrix_make_identity(a);

    a[axis1][axis1] =   cs;
    a[axis1][axis2] = -sn;
    a[axis2][axis1] =   sn;
    a[axis2][axis2] =  cs;

    return 1;
}

void matrix_make_movement_sequence_matrix(double v[N_DIM+1][N_DIM+1], double vi[N_DIM+1][N_DIM+1],
        int n, int mtype[], int maxis[], double mparam[])
{
    double ftmp[N_DIM+1][N_DIM+1], btmp[N_DIM+1][N_DIM+1];
    double ptmp[N_DIM];

    matrix_make_identity(v);
    matrix_make_identity(vi);

    for(int i=0; i<n; ++i)
    {
        switch(mtype[i]) {
        case(SCALE):
        {
            for(int j=0; j<N_DIM; ++j) {
                ptmp[j] = 1.0;
            }
            ptmp[maxis[i]] = mparam[i];
            matrix_make_scaling(ftmp, ptmp);
            ptmp[maxis[i]] = 1.0/mparam[i];
            matrix_make_scaling(btmp, ptmp);
            break;
        }
        case(ROTATE):
        {
            const double angle = mparam[i];
            const int axis1 = maxis[i] % N_DIM;
            const int axis2 = maxis[i] / N_DIM;
            matrix_make_rotation_cs(ftmp, axis1, axis2, cos(angle), sin(angle));
            matrix_make_rotation_cs(btmp, axis1, axis2, cos(-angle), sin(-angle));
            break;
        }

        case(TRANSLATE):
        {
            for(int j=0; j<N_DIM; ++j) {
                ptmp[j] = 0.0;
            }
            ptmp[maxis[i]] = mparam[i];
            matrix_make_translation(ftmp, ptmp);
            ptmp[maxis[i]] = -mparam[i];
            matrix_make_translation(btmp, ptmp);
            break;
        }
        }
        matrix_mult(v, ftmp, v);
        matrix_mult(vi, vi, btmp);
    }
}

void matrix_make_view(double v[N_DIM+1][N_DIM+1], double vi[N_DIM+1][N_DIM+1],  double eyeA[N_DIM], double coiA[N_DIM], double upA[N_DIM])
{

    int i = 0;
    int mtype[100];
    int maxis[100];
    double mparam[100];

    double new_coiA[N_DIM], new_upA[N_DIM];

    // translate eye to origin
    for(int axis = 0; axis < N_DIM; axis++) {
        mtype[i] = TRANSLATE;
        maxis[i] = axis;
        mparam[i] =  -eyeA[axis];
        i++;
    }

    // neutralize all coi axes except the last one
    for(int axis = 0; axis < N_DIM-1; axis++) {
        // update new_coiA to reflect its new position
        matrix_make_movement_sequence_matrix(v,vi,  i,mtype,maxis,mparam);
        matrix_mult_pt(new_coiA, v, coiA);

        mtype[i] = ROTATE;
        // rotate about (axis/z plane)
        maxis[i] = axis * N_DIM + N_DIM-1;
        mparam[i] = atan2(new_coiA[axis], new_coiA[N_DIM-1]);
        i++;
    }

    // rotate up vector to point along y-axis
    // neutralize all up axes except the second to last one
    for(int axis = 0; axis < N_DIM-2; axis++) {
        // update new_coiA to reflect its new position
        matrix_make_movement_sequence_matrix(v,vi,  i,mtype,maxis,mparam);
        matrix_mult_pt(new_upA, v, upA);

        mtype[i] = ROTATE;
        // rotate about (axis/y plane)
        maxis[i] = axis * N_DIM + N_DIM-2;
        mparam[i] = atan2(new_upA[axis], new_upA[N_DIM-2]);
        i++;
    }

}
