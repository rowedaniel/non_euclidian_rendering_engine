#include "vector.h"
#include <stdio.h>
#include <assert.h>

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

void test_vec2_add(void) {
    Vector2 vec1 = {{0.5, 0.0}};
    Vector2 vec2 = {{123.45, 5.0}};
    Vector2 res = vec2_add(vec1, vec2);
    assert(res.data[0] == 123.95);
    assert(res.data[1] == 5.0);
    printf(".");
}

void test_vec3_add(void) {
    Vector3 vec1 = {{0.5, 0.0, 2.055}};
    Vector3 vec2 = {{123.45, 5.0, 2.055}};
    Vector3 res = vec3_add(vec1, vec2);
    assert(res.data[0] == 123.95);
    assert(res.data[1] == 5.0);
    assert(res.data[2] == 4.11);
    printf(".");
}

int main() {
    test_print_vector2d();
    test_print_vector2();
    test_print_vector3();
    test_vec2_add();
    test_vec3_add();
}
