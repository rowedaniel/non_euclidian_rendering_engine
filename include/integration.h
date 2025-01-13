/**
 * @file integration.h
 * @brief collection of numerical integration techniques
 * @author Daniel Neshyba-Rowe
 */

#include "dim.h"

/**
 * @struct Christoffel
 * @brief Christoffel symbols
*/
typedef union ChristoffelSymbols {
    double symols[N_DIM][N_DIM][N_DIM];
#if N_DIM == 2
    struct {
        double d2t_dtau2, d2x_dtau2;
    }
#elif N_DIM == 3
    struct {
        double d2t_dtau2, d2x_dtau2;
    }
#elif N_DIM == 4
#endif
} ChristoffelSymbols;
