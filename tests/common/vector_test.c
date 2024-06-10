#include "vector.h"
#include <stdio.h>

void test_print_vector2d(void) {
    Vector2d vec = {{100, -3}};
    printf("100 -3 = ");
    print_vector2d(vec);
}

void test_print_vector2(void) {
    Vector2 vec = {{0.0, -0.5}};
    printf("0.000000 -0.500000 = ");
    print_vector2(vec);
}

void test_print_vector3(void) {
    Vector3 vec = {{0.0, 5.0, 2.055}};
    printf("0.000000 5.000000 2.055000 = ");
    print_vector3(vec);
}

int main() {
    test_print_vector2d();
    test_print_vector2();
    test_print_vector3();
}
