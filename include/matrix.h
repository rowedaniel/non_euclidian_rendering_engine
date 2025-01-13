/**
 * @file matrix.h
 * @brief a collection of tools for vectors and matrices
 * @author Daniel Neshyba-Rowe
 */

#include "dim.h"
#include "vector.h"

typedef union MatrixN {
    double rows[N_DIM][N_DIM];
    double data[N_DIM*N_DIM];
} MatrixN;

typedef union AffineMatrixN {
    double rows[N_DIM+1][N_DIM+1];
    double data[(N_DIM+1)*(N_DIM+1)];
} AffineMatrixN;

/**
 * @brief Given matricies A,B of same dimension as space, gives AB
 * @param A LHS matrix
 * @param B RHS matrix
 * @return AB
*/
MatrixN matrixN_product(MatrixN A, MatrixN B);

/**
 * @brief Print an matrix
 * @param matrix An matrix
*/
void matrixN_print(MatrixN matrix);



/**
 * @brief Given affine matricies A,B of same dimension as space, gives AB
 * @param A LHS affine matrix
 * @param B RHS affine matrix
 * @return AB
*/
AffineMatrixN affinematrixN_product(AffineMatrixN A, AffineMatrixN B);

/**
 * @brief Print an affine matrix
 * @param matrix An affine matrix
*/
void affinematrixN_print(AffineMatrixN matrix);


