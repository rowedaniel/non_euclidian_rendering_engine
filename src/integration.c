#include "integration.h"

void euler(
        VectorN pos,
        VectorN direction,
        double deltatau,
        ChristoffelSymbols symbs
        ) {
    VectorN second_deriv;
    for(int i=0; i<N_DIM; ++i) {
        second_deriv[i] = vectorN_dot(matrixN_product
    }
}

