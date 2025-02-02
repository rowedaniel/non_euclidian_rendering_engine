/**
 * @file integration.h
 * @brief collection of numerical integration techniques
 * @author Daniel Neshyba-Rowe
 */

#pragma once
#include "dim.h"
#include "vector.h"
#include "matrix.h"

/**
 * @struct Christoffel
 * @brief Christoffel symbols
*/
typedef union ChristoffelSymbols {
    double symbol_data[N_DIM][N_DIM][N_DIM];
    MatrixN symbols[N_DIM];
#if N_DIM == 2
    struct {
        double d2t_dtau2, d2x_dtau2;
    };
#elif N_DIM == 3
    struct {
        double d2t_dtau2, d2x_dtau2, d2y_dtau2;
    };
#elif N_DIM == 4
    struct {
        double d2t_dtau2, d2x_dtau2, d2y_dtau2, d2z_dtau2;
    };
#endif
} ChristoffelSymbols;


/**
 * @brief numerical integration using Euler's method
 * @param pos the start position
 * @param direction direction vector
 * @param deltatau parametrized time to integrate for
 * modifies pos and direction values in-place
*/
void euler(VectorN pos, VectorN direction, double deltatau, ChristoffelSymbols symbs);
