#include "vector.h"
#include <stdio.h>

void test_print_vectorN(void) {
    VectorN vec = {{1.0, 2.0, 0.0}};
    printf("1.000000 2.000000 0.000000 = ");
    print_vectorN(vec);
}

int main() {
    test_print_vectorN();
}
