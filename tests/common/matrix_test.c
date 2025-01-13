#include "matrix.h"
#include <stdio.h>
#include <assert.h>

void test_matrix_basic(void) {
    MatrixN test;
    test.rows[0][0] = 1.0;
    assert(test.rows[0][0] == 1.0);
    printf(".\n");
}

void test_matrixN_print(void) {
    MatrixN matrix = {{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }
    };
    matrixN_print(matrix);
}

void test_affinematrixN_product(void) {
    AffineMatrixN A = {{
            {1, 0, 0, 1},
            {0, 1, 0, 2},
            {0, 0, 1, 3},
            {0, 0, 0, 1}
        }
    };
    AffineMatrixN B = {{
            {1, 2, 3, 0},
            {4, 5, 6, 0},
            {7, 8, 9, 0},
            {0, 0, 0, 1}
        }
    };
    AffineMatrixN C = affinematrixN_product(A, B);

    AffineMatrixN desired = {{
            {1, 2, 3, 1},
            {4, 5, 6, 2},
            {7, 8, 9, 3},
            {0, 0, 0, 1}
        }
    };

    for(int i=0; i<N_DIM*N_DIM; ++i)
        assert(C.data[i] == desired.data[i]);

    printf(".\n");
}

void test_matrixN_product(void) {
    MatrixN A = {{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }
    };
    MatrixN B = {{
            {4, 5, 6},
            {7, 8, 9},
            {1, 2, 3}
        }
    };
    MatrixN C = matrixN_product(A, B);

    MatrixN desired = {{
            { 21, 27,  33},
            { 57, 72,  87},
            { 93, 117, 141}
        }
    };

    for(int i=0; i<N_DIM*N_DIM; ++i)
        assert(C.data[i] == desired.data[i]);

    printf(".\n");
}




int main(void) {
    // TODO: make this compatible with testing setup
    //test_matrixN_print();
    test_matrix_basic();
    test_matrixN_product();
    test_affinematrixN_product();
}
