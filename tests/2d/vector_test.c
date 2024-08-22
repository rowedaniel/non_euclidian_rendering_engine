#include "vector.h"
#include <stdio.h>
#include <assert.h>

void test_vectorN_print(void) {
    VectorN vec = {{1.0, 0.0}};
    printf("1.000000 0.000000 =");
    vectorN_print(vec);
}

void test_vectorN_dot(void) {
    VectorN vec1 = {{10.0, 0.5}};
    VectorN vec2 = {{0.0, 2.0}};
    assert(vectorN_dot(vec1, vec2) == 1.0);
    printf(".\n");
}

int main(void) {
    test_vectorN_print();
    test_vectorN_dot();
}

