#include "matrix.h"
#include <stdio.h>

void matrix_print(double data[], int size) {
    int i,j;
    for(i=0; i<size; ++i) {
        printf("[ ");
        for(j=0; j<size-1; ++j) {
            printf("%lf \t", data[i*size + j]);
        }
        printf("%lf ]\n", data[i*size + j]);
    }
}

void matrix_product(double data1[], double data2[], int size, double data_out[]) {
    for(int r=0; r<size; ++r) {
        for(int c=0; c<size; ++c) {
            double sum = 0.0;
            for(int k=0; k<size; ++k)
                sum += data1[r*size + k] * data2[k*size + c];
            data_out[r* size + c] = sum;
        }
    }
}

void matrixN_print(MatrixN matrix) {
    matrix_print(matrix.data, N_DIM);
}

MatrixN matrixN_product(MatrixN A, MatrixN B) {
    MatrixN out;
    matrix_product(A.data, B.data, N_DIM, out.data);
    return out;
}

void affinematrixN_print(AffineMatrixN matrix) {
    matrix_print(matrix.data, N_DIM+1);
}

AffineMatrixN affinematrixN_product(AffineMatrixN A, AffineMatrixN B) {
    AffineMatrixN out;
    matrix_product(A.data, B.data, N_DIM+1, out.data);
    return out;
}
