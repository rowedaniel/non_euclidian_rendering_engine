#include "vector.h"
#include <stdio.h>
#include <assert.h>

void test_vector2_basic(void) {
    Vector3 vec = {{0.0, 1.0}};
    assert(vec.r == 0.0 && vec.x == 0.0);
    assert(vec.g == 1.0 && vec.y == 1.0);
    printf(".\n");
}

void test_vector3_basic(void) {
    Vector3 vec = {{0.0, 1.0, 100.123}};
    assert(vec.r == 0.0 && vec.x == 0.0);
    assert(vec.g == 1.0 && vec.y == 1.0);
    assert(vec.b == 100.123 && vec.z == 100.123);
    printf(".\n");
}

void test_vector2d_print(void) {
    Vector2d vec = {{100, -3}};
    printf("100 -3 =");
    vector2d_print(vec);
}

void test_vector2_print(void) {
    Vector2 vec = {{0.0, -0.5}};
    printf("0.000000 -0.500000 =");
    vector2_print(vec);
}

void test_vector3_print(void) {
    Vector3 vec = {{0.0, 5.0, 2.055}};
    printf("0.000000 5.000000 2.055000 =");
    vector3_print(vec);
}

void test_vector2_add(void) {
    Vector2 vec1 = {{0.5, 0.0}};
    Vector2 vec2 = {{123.45, 5.0}};
    Vector2 res = vector2_add(vec1, vec2);
    assert(res.data[0] == 123.95);
    assert(res.data[1] == 5.0);
    printf(".\n");
}

void test_vector3_add(void) {
    Vector3 vec1 = {{0.5, 0.0, 2.055}};
    Vector3 vec2 = {{123.45, 5.0, 2.055}};
    Vector3 res = vector3_add(vec1, vec2);
    assert(res.data[0] == 123.95);
    assert(res.data[1] == 5.0);
    assert(res.data[2] == 4.11);
    printf(".\n");
}

void test_dot(void) {
    double vec1[] = {10.0, 0.5, 0.0};
    double vec2[] = {0.0, 2.0, 0.123};
    assert(dot(vec1, vec2, 3) == 1.0);
    printf(".\n");
}

void test_vector2_dot(void) {
    Vector2 vec1 = {{10.0, 0.5}};
    Vector2 vec2 = {{0.0, 2.0}};
    assert(vector2_dot(vec1, vec2) == 1.0);
    printf(".\n");
}

void test_vector3_dot(void) {
    Vector3 vec1 = {{10.0, 0.5, 0.0}};
    Vector3 vec2 = {{0.0, 2.0, 0.123}};
    assert(vector3_dot(vec1, vec2) == 1.0);
    printf(".\n");
}

void test_vector3_cross(void) {
    Vector3 vec1 = {{10.0, 0.5, 0.0}};
    Vector3 vec2 = {{0.0, 2.0, 0.123}};
    Vector3 cross = vector3_cross(vec1, vec2);
    assert(cross.x == 0.0615);
    assert(cross.y == -1.23);
    assert(cross.z == 20.0);
    printf(".\n");
}

int main(void) {
    test_vector2_basic();
    test_vector3_basic();
    test_vector2d_print();
    test_vector2_print();
    test_vector3_print();
    test_vector2_add();
    test_vector3_add();
    test_dot();
    test_vector2_dot();
    test_vector3_dot();
    test_vector3_cross();
}
