#include "vector.h"
#include <stdio.h>
#include <assert.h>

void test_print_vectorN(void) {
    VectorN vec = {{1.0, 2.0, 0.0}};
    printf("1.000000 2.000000 0.000000 =");
    print_vectorN(vec);
}

void test_vecN_dot(void) {
    VectorN vec1 = {{10.0, 0.5, 0.0}};
    VectorN vec2 = {{0.0, 2.0, 0.123}};
    assert(vecN_dot(vec1, vec2) == 1.0);
    printf(".\n");
}


int main(void) {
    test_print_vectorN();
    test_vecN_dot();
}
